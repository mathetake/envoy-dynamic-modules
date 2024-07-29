package main

import (
	"bytes"
	_ "embed"
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"net/http"
	"net/http/httptest"
	"os"
	"os/exec"
	"strings"
	"sync"
	"time"

	"github.com/stretchr/testify/require"
)

//go:embed envoy.yaml
var envoyYaml string

var (
	stdOut                    *bytes.Buffer
	testUpstreamHandler       = map[string]http.HandlerFunc{}
	testUpstreamHeandlerMutex sync.Mutex
)

// T implements require.TestingT.
type T struct {
	name string
}

func (t *T) Errorf(format string, args ...interface{}) {
	log.Printf("[%s] %s", t.name, fmt.Sprintf(format, args...))
}

func (t *T) FailNow() { panic("FailNow") }

var (
	target            = flag.String("target", "", "target to test")
	sharedLibraryPath = flag.String("shared-library-path", "./main", "path to the shared library to test")
)

func main() {
	flag.Parse()

	envoyYamlTmp, err := os.CreateTemp("", "*.yaml")
	if err != nil {
		log.Panicf("failed to create temp dir: %v", err)
	}
	defer os.RemoveAll(envoyYamlTmp.Name())

	t := &T{name: "main"}

	// Check if a binary named main exists.
	derefSharedLibraryPath := *sharedLibraryPath
	_, err = os.Stat(derefSharedLibraryPath)
	require.NoErrorf(t, err, "shared library at %s not found. Please build it.", derefSharedLibraryPath)

	// Replace SHARED_LIBRARY_PATH with the actual path to the shared library.
	envoyYaml = strings.ReplaceAll(string(envoyYaml), "SHARED_LIBRARY_PATH", derefSharedLibraryPath)

	// Write the envoy.yaml file to the temp directory.
	if _, err := envoyYamlTmp.WriteString(envoyYaml); err != nil {
		log.Panicf("failed to write envoy.yaml: %v", err)
	}

	l, err := net.Listen("tcp", "127.0.0.1:8199")
	require.NoError(t, err)
	testUpstream := &httptest.Server{
		Listener: l,
		Config: &http.Server{Handler: http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
			value := r.Header.Get("go-sdk-test-case")
			testUpstreamHeandlerMutex.Lock()
			defer testUpstreamHeandlerMutex.Unlock()
			hander, ok := testUpstreamHandler[value]
			if !ok {
				log.Printf("testUpstreamHandler not found for %s", value)
				w.WriteHeader(http.StatusNotFound)
				return
			}
			hander(w, r)
		})},
	}
	testUpstream.Start()
	defer testUpstream.Close()

	// Check if `envoy` is installed.
	_, err = exec.LookPath("envoy")
	require.NoError(t, err, "envoy binary not found. Please install it from containers at https://github.com/mathetake/envoy-dynamic-modules/pkgs/container/envoy")

	cmd := exec.Command("envoy", "--concurrency", "1", "-c", envoyYamlTmp.Name())
	stdOut = new(bytes.Buffer)
	cmd.Stdout = stdOut
	cmd.Stderr = os.Stderr
	require.NoError(t, cmd.Start())
	defer func() { require.NoError(t, cmd.Process.Kill()) }()

	if *target != "" {
		if test, ok := testCases[*target]; ok {
			log.Printf("Running test %s", *target)
			test(&T{name: *target})
			log.Printf("Test %s passed", *target)
		} else {
			log.Panicf("test case %s not found", *target)
		}
	} else {
		var wg sync.WaitGroup
		wg.Add(len(testCases))
		for name, test := range testCases {
			name, test := name, test
			go func() {
				defer wg.Done()
				log.Printf("Running test %s", name)
				test(&T{name: name})
				log.Printf("Test %s passed", name)
			}()
		}
		wg.Wait()
	}
}

var testCases = map[string]func(*T){
	"TestHelloWorld":    TestHelloWorld,
	"TestHeaders":       TestHeaders,
	"TestDelay":         TestDelayFilter,
	"TestBodies":        TestBodies,
	"TestBodiesReplace": TestBodiesReplace,
	"TestSendReplay":    TestSendReplay,
}

func TestHeaders(t *T) {
	testUpstreamHeandlerMutex.Lock()
	testUpstreamHandler["headers"] = func(w http.ResponseWriter, r *http.Request) {
		require.Equal(t, "yes", r.Header.Get("foo"))
		require.Equal(t, "", r.Header.Get("multiple-values"))
		require.Equal(t, []string{"single"}, r.Header.Values("multiple-values-to-be-single"))

		w.Header().Set("this-is", "response-header")
		w.Header().Add("this-is-2", "A")
		w.Header().Add("this-is-2", "B")
		w.Header().Set("multiple-values2-to-be-single", "A")
		w.Header().Add("multiple-values2-to-be-single", "B")
		w.WriteHeader(http.StatusOK)
	}
	testUpstreamHeandlerMutex.Unlock()

	require.Eventually(t, func() bool {
		req, err := http.NewRequest("GET", "http://localhost:15002", bytes.NewBufferString("hello"))
		if err != nil {
			return false
		}
		req.Header.Set("go-sdk-test-case", "headers")
		req.Header.Set("foo", "value")
		req.Header.Add("multiple-values", "1234")
		req.Header.Add("multiple-values", "next")

		req.Header.Add("multiple-values2-to-be-single", "A")
		req.Header.Add("multiple-values2-to-be-single", "B")

		res, err := http.DefaultClient.Do(req)
		if err != nil {
			return false
		}
		defer res.Body.Close()
		if res.StatusCode != http.StatusOK {
			return false
		}

		// Check if the response headers are as expected.
		if res.Header.Get("this-is") != "response-header" {
			fmt.Println("this-is:", res.Header.Values("this-is"))
			return false
		}
		if res.Header.Values("this-is-2") != nil {
			fmt.Println("this-is-2:", res.Header.Values("this-is-2"))
			return false
		}
		if toBeSingle := res.Header.Values("multiple-values-res-to-be-single"); len(toBeSingle) != 1 || toBeSingle[0] != "single" {
			fmt.Println("multiple-values-to-be-single:", toBeSingle)
			return false
		}
		return true
	}, 10*time.Second, 2*time.Second, "Envoy has not started: %s", stdOut.String())

	// Check if the log contains the expected output.
	requireEventuallyContainsMessages(t, stdOut,
		"foo: value",
		"multiple-values: 1234",
		"multiple-values: next",
		"this-is: response-header",
		"this-is-2: A",
		"this-is-2: B",
	)
}

func TestDelayFilter(t *T) {
	// Make four requests to the envoy proxy.
	wg := new(sync.WaitGroup)
	wg.Add(4)
	for i := 0; i < 4; i++ {
		go func() {
			defer wg.Done()
			require.Eventually(t, func() bool {
				req, err := http.NewRequest("GET", "http://localhost:15001", bytes.NewBufferString("hello"))
				if err != nil {
					return false
				}
				res, err := http.DefaultClient.Do(req)
				if err != nil {
					return false
				}
				defer res.Body.Close()
				return res.StatusCode == http.StatusOK
			}, 10*time.Second, 2*time.Second, "Envoy has not started: %s", stdOut.String())
		}()
	}
	wg.Wait()

	// Check if the log contains the expected output.
	requireEventuallyContainsMessages(t, stdOut,
		"RequestHeaders returning StopAllIterationAndBuffer with id 1",
		"blocking for 1 second at RequestHeaders with id 1",
		"calling ContinueRequest with id 1",
		"RequestBody called with id 1",
		"ResponseHeaders called with id 1",
		"RequestHeaders called with id 2",
		"RequestBody returning StopIterationAndBuffer with id 2",
		"blocking for 1 second at RequestBody with id 2",
		"calling ContinueRequest with id 2",
		"ResponseBody called with id 2",
		"RequestHeaders called with id 3",
		"RequestBody called with id 3",
		"ResponseHeaders returning StopAllIterationAndBuffer with id 3",
		"blocking for 1 second at ResponseHeaders with id 3",
		"calling ContinueResponse with id 3",
		"ResponseBody called with id 3",
		"RequestHeaders called with id 4",
		"RequestBody called with id 4",
		"ResponseHeaders called with id 4",
		"blocking for 1 second at ResponseBody with id 4",
		"ResponseBody returning StopIterationAndBuffer with id 4",
		"calling ContinueResponse with id 4",
	)
}

func TestHelloWorld(t *T) {
	require.Eventually(t, func() bool {
		req, err := http.NewRequest("GET", "http://localhost:15000", bytes.NewBufferString("hello"))
		if err != nil {
			return false
		}
		res, err := http.DefaultClient.Do(req)
		if err != nil {
			return false
		}
		defer res.Body.Close()
		return res.StatusCode == http.StatusOK
	}, 5*time.Second, 100*time.Millisecond, "Envoy has not started: %s", stdOut.String())

	// Check if the log contains the expected output.
	requireEventuallyContainsMessages(t, stdOut,
		"RequestHeaders called",
		"RequestBody called",
		"ResponseHeaders called",
		"ResponseBody called",
		"Destroy called",
	)
}

func TestBodies(t *T) {
	testUpstreamHeandlerMutex.Lock()
	testUpstreamHandler["bodies"] = func(w http.ResponseWriter, r *http.Request) {
		defer r.Body.Close()
		body, err := io.ReadAll(r.Body)
		require.NoError(t, err)
		require.Equal(t, "XXXXXXXXXX", string(body)) // Request body should be replaced with 'X'.

		w.Header().Set("Content-Type", "text/plain")
		w.WriteHeader(http.StatusOK)
		_, err = w.Write([]byte("example body\n"))
		require.NoError(t, err)
	}
	testUpstreamHeandlerMutex.Unlock()

	require.Eventually(t, func() bool {
		req, err := http.NewRequest("GET", "http://localhost:15003", bytes.NewBufferString("0123456789"))
		if err != nil {
			return false
		}
		req.Header.Set("go-sdk-test-case", "bodies")
		res, err := http.DefaultClient.Do(req)
		if err != nil {
			return false
		}
		defer res.Body.Close()

		resBody, err := io.ReadAll(res.Body)
		if err != nil {
			return false
		}
		require.Equal(t, "YYYYYYYYYYYYY", string(resBody))
		return res.StatusCode == http.StatusOK
	}, 5*time.Second, 100*time.Millisecond, "Envoy has not started: %s", stdOut.String())

	// Check if the log contains the expected output.
	requireEventuallyContainsMessages(t, stdOut,
		"entire request body: 0123456789",
		"request body read 2 bytes offset at 0: \"01\"",
		"request body read 2 bytes offset at 2: \"23\"",
		"request body read 2 bytes offset at 4: \"45\"",
		"request body read 2 bytes offset at 6: \"67\"",
		"request body read 2 bytes offset at 8: \"89\"",

		"entire response body: example body",
		"response body read 2 bytes offset at 0: \"ex\"",
		"response body read 2 bytes offset at 2: \"am\"",
		"response body read 2 bytes offset at 4: \"pl\"",
		"response body read 2 bytes offset at 6: \"e \"",
		"response body read 2 bytes offset at 8: \"bo\"",
		"response body read 2 bytes offset at 10: \"dy\"",
	)
}

func TestBodiesReplace(t *T) {
	testUpstreamHeandlerMutex.Lock()
	testUpstreamHandler["bodies_replace"] = func(w http.ResponseWriter, r *http.Request) {
		defer r.Body.Close()
		body, err := io.ReadAll(r.Body)
		require.NoError(t, err)
		expRequestBody := r.Header.Get("exp-request-body")
		require.Equal(t, expRequestBody, string(body))

		if append := r.Header.Get("response-append"); append != "" {
			w.Header().Set("append", append)
		}
		if prepend := r.Header.Get("response-prepend"); prepend != "" {
			w.Header().Set("prepend", prepend)
		}
		if replace := r.Header.Get("response-replace"); replace != "" {
			w.Header().Set("replace", replace)
		}
		w.Header().Set("Content-Type", "text/plain")
		w.WriteHeader(http.StatusOK)

		_, err = w.Write([]byte("[upstream body]"))
		require.NoError(t, err)
	}
	testUpstreamHeandlerMutex.Unlock()

	cases := []struct {
		requestAppend, requestPrepend, requestReplace    string
		responseAppend, responsePrepend, responseReplace string
		expRequestBody, expResponseBody                  string
	}{
		{
			requestAppend: "AAAAAA", expRequestBody: "[downstream body]AAAAAA",
			responseAppend: "BBBBBB", expResponseBody: "[upstream body]BBBBBB",
		},
		{
			requestPrepend: "AAAAAA", expRequestBody: "AAAAAA[downstream body]",
			responsePrepend: "BBBBBB", expResponseBody: "BBBBBB[upstream body]",
		},
		{
			requestReplace: "AAAAAA", expRequestBody: "AAAAAA",
			responseReplace: "BBBBBB", expResponseBody: "BBBBBB",
		},
		{
			requestAppend: "AAAAAA", requestPrepend: "BBBBBB", expRequestBody: "BBBBBB[downstream body]AAAAAA",
			responseAppend: "CCCCC", responsePrepend: "DDDDD", expResponseBody: "DDDDD[upstream body]CCCCC",
		},
	}

	var wg sync.WaitGroup
	wg.Add(len(cases))
	for _, tc := range cases {
		tc := tc
		go func() {
			defer wg.Done()
			require.Eventually(t, func() bool {
				req, err := http.NewRequest("GET", "http://localhost:15004", bytes.NewBufferString("[downstream body]"))
				if err != nil {
					return false
				}
				req.Header.Set("go-sdk-test-case", "bodies_replace")

				if tc.requestAppend != "" {
					req.Header.Set("append", tc.requestAppend)
				}
				if tc.requestPrepend != "" {
					req.Header.Set("prepend", tc.requestPrepend)
				}
				if tc.requestReplace != "" {
					req.Header.Set("replace", tc.requestReplace)
				}
				if tc.responseAppend != "" {
					req.Header.Set("response-append", tc.responseAppend)
				}
				if tc.responsePrepend != "" {
					req.Header.Set("response-prepend", tc.responsePrepend)
				}
				if tc.responseReplace != "" {
					req.Header.Set("response-replace", tc.responseReplace)
				}
				req.Header.Set("exp-request-body", tc.expRequestBody)
				res, err := http.DefaultClient.Do(req)
				if err != nil {
					return false
				}
				defer res.Body.Close()

				resBody, err := io.ReadAll(res.Body)
				if err != nil {
					return false
				}
				require.Equal(t, tc.expResponseBody, string(resBody))
				return res.StatusCode == http.StatusOK
			}, 5*time.Second, 100*time.Millisecond, "Envoy has not started: %s", stdOut.String())
		}()
	}
	wg.Wait()
}

func TestSendReplay(t *T) {
	wg := sync.WaitGroup{}
	wg.Add(2)

	go func() {
		defer wg.Done()
		require.Eventually(t, func() bool {
			req, err := http.NewRequest("GET", "http://localhost:15005/on_request", nil)
			if err != nil {
				return false
			}
			res, err := http.DefaultClient.Do(req)
			if err != nil {
				return false
			}
			defer res.Body.Close()

			resBody, err := io.ReadAll(res.Body)
			if err != nil {
				return false
			}
			fmt.Println(res.Header)
			fmt.Println(string(resBody))
			require.Equal(t, "bar", res.Header.Get("foo"))
			require.Equal(t, "baz", res.Header.Get("bar"))
			require.Equal(t, "local response at request headers", string(resBody))
			return res.StatusCode == http.StatusUnauthorized
		}, 5*time.Second, 100*time.Millisecond, "Envoy has not started: %s", stdOut.String())
	}()

	go func() {
		defer wg.Done()
		require.Eventually(t, func() bool {
			req, err := http.NewRequest("GET", "http://localhost:15005/on_response", nil)
			if err != nil {
				return false
			}
			res, err := http.DefaultClient.Do(req)
			if err != nil {
				return false
			}
			defer res.Body.Close()

			resBody, err := io.ReadAll(res.Body)
			if err != nil {
				return false
			}
			fmt.Println(res.Header)
			fmt.Println(string(resBody))
			require.Equal(t, "cat", res.Header.Get("dog"))
			require.Equal(t, "local response at response headers", string(resBody))
			return res.StatusCode == http.StatusInternalServerError
		}, 5*time.Second, 100*time.Millisecond, "Envoy has not started: %s", stdOut.String())
	}()
	wg.Wait()
}

func requireEventuallyContainsMessages(t *T, buf *bytes.Buffer, messages ...string) {
	for _, msg := range messages {
		require.Eventually(t, func() bool {
			return strings.Contains(buf.String(), msg)
		}, 3*time.Second, 100*time.Millisecond, "Expected message \"%s\" not found in buffer\n%s", msg, buf.String())
	}
}
