package main

import (
	"fmt"
	"io"

	"github.com/mathetake/envoy-dynamic-modules-go-sdk/envoy"
)

// bodiesHttpFilter implements envoy.HttpFilter.
//
// This is to demonstrate how to use body manipulation APIs.
type bodiesHttpFilter struct{}

func newbodiesHttpFilter(string) envoy.HttpFilter { return &bodiesHttpFilter{} }

// NewInstance implements envoy.HttpFilter.
func (f *bodiesHttpFilter) NewInstance(envoyFilter envoy.EnvoyFilterInstance) envoy.HttpFilterInstance {
	return &bodiesHttpFilterInstance{envoyFilter: envoyFilter}
}

// Destroy implements envoy.HttpFilter.
func (f *bodiesHttpFilter) Destroy() {}

// bodiesHttpFilterInstance implements envoy.HttpFilterInstance.
type bodiesHttpFilterInstance struct {
	envoyFilter envoy.EnvoyFilterInstance
}

// RequestHeaders implements envoy.HttpFilterInstance.
func (h *bodiesHttpFilterInstance) RequestHeaders(envoy.RequestHeaders, bool) envoy.RequestHeadersStatus {
	return envoy.HeadersStatusContinue
}

// RequestBody implements envoy.HttpFilterInstance.
func (h *bodiesHttpFilterInstance) RequestBody(frame envoy.RequestBodyBuffer, endOfStream bool) envoy.RequestBodyStatus {
	fmt.Printf("new request body frame: %s\n", string(frame.Copy()))
	if !endOfStream {
		// Wait for the end of the stream to see the full body.
		return envoy.RequestBodyStatusStopIterationAndBuffer
	}

	// Now we can read the entire body.
	entireBody := h.envoyFilter.GetRequestBodyBuffer()

	// This copies the entire body into a single contiguous buffer in Go.
	fmt.Printf("entire request body: %s", string(entireBody.Copy()))

	// This demonstrates how to use ReadAt to read the body at a specific offset.
	var buf [2]byte
	for i := 0; ; i += 2 {
		_, err := entireBody.ReadAt(buf[:], int64(i))
		if err == io.EOF {
			break
		}
		fmt.Printf("request body read 2 bytes offset at %d: \"%s\"\n", i, string(buf[:]))
	}

	// Replace the entire body with 'X' without copying.
	entireBody.Slices(func(view []byte) {
		for i := 0; i < len(view); i++ {
			view[i] = 'X'
		}
	})
	return envoy.RequestBodyStatusContinue
}

// ResponseHeaders implements envoy.HttpFilterInstance.
func (h *bodiesHttpFilterInstance) ResponseHeaders(envoy.ResponseHeaders, bool) envoy.ResponseHeadersStatus {
	return envoy.ResponseHeadersStatusContinue
}

// ResponseBody implements envoy.HttpFilterInstance.
func (h *bodiesHttpFilterInstance) ResponseBody(frame envoy.ResponseBodyBuffer, endOfStream bool) envoy.ResponseBodyStatus {
	fmt.Printf("new request body frame: %s\n", string(frame.Copy()))
	if !endOfStream {
		// Wait for the end of the stream to see the full body.
		return envoy.ResponseBodyStatusStopIterationAndBuffer
	}

	// Now we can read the entire body.
	entireBody := h.envoyFilter.GetResponseBodyBuffer()

	// This copies the entire body into a single contiguous buffer in Go.
	fmt.Printf("entire response body: %s", string(entireBody.Copy()))

	// This demonstrates how to use ReadAt to read the body at a specific offset.
	var buf [2]byte
	for i := 0; ; i += 2 {
		_, err := entireBody.ReadAt(buf[:], int64(i))
		if err == io.EOF {
			break
		}
		fmt.Printf("response body read 2 bytes offset at %d: \"%s\"\n", i, string(buf[:]))
	}

	// Replace the entire body with 'Y' without copying.
	entireBody.Slices(func(view []byte) {
		for i := 0; i < len(view); i++ {
			view[i] = 'Y'
		}
	})
	return envoy.ResponseBodyStatusContinue
}

// Destroy implements envoy.HttpFilterInstance.
func (h *bodiesHttpFilterInstance) Destroy() {}
