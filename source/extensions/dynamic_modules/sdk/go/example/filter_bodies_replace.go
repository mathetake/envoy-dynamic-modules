package main

import (
	"fmt"

	"github.com/mathetake/envoy-dynamic-modules/source/extensions/dynmic_modules/sdk/go/envoy"
)

// bodiesReplaceHttpFilter implements envoy.HttpFilter.
//
// This is to demonstrate how to use body manipulation APIs.
type bodiesReplaceHttpFilter struct{}

func newbodiesReplaceHttpFilter(string) envoy.HttpFilter { return &bodiesReplaceHttpFilter{} }

// NewInstance implements envoy.HttpFilter.
func (f *bodiesReplaceHttpFilter) NewInstance(envoyFilter envoy.EnvoyFilterInstance) envoy.HttpFilterInstance {
	return &bodiesReplaceHttpFilterInstance{envoyFilter: envoyFilter}
}

// Destroy implements envoy.HttpFilter.
func (f *bodiesReplaceHttpFilter) Destroy() {}

// bodiesReplaceHttpFilterInstance implements envoy.HttpFilterInstance.
type bodiesReplaceHttpFilterInstance struct {
	envoyFilter                                      envoy.EnvoyFilterInstance
	requestAppend, requestPrepend, requestReplace    string
	responseAppend, responsePrepend, responseReplace string
}

// RequestHeaders implements envoy.HttpFilterInstance.
func (h *bodiesReplaceHttpFilterInstance) RequestHeaders(headers envoy.RequestHeaders, _ bool) envoy.RequestHeadersStatus {
	append, ok := headers.Get("append")
	if ok {
		h.requestAppend = append.String()
	}
	prepend, ok := headers.Get("prepend")
	if ok {
		h.requestPrepend = prepend.String()
	}
	replace, ok := headers.Get("replace")
	if ok {
		h.requestReplace = replace.String()
	}
	headers.Remove("content-length") // Remove the content-length header to reset the length.
	return envoy.HeadersStatusContinue
}

// RequestBody implements envoy.HttpFilterInstance.
func (h *bodiesReplaceHttpFilterInstance) RequestBody(frame envoy.RequestBodyBuffer, endOfStream bool) envoy.RequestBodyStatus {
	if !endOfStream {
		// Wait for the end of the stream to see the full body.
		return envoy.RequestBodyStatusStopIterationAndBuffer
	}

	entireBody := h.envoyFilter.GetRequestBodyBuffer()
	if h.requestAppend != "" {
		entireBody.Append([]byte(h.requestAppend))
	}
	if h.requestPrepend != "" {
		entireBody.Prepend([]byte(h.requestPrepend))
	}
	if h.requestReplace != "" {
		entireBody.Replace([]byte(h.requestReplace))
	}
	return envoy.RequestBodyStatusContinue
}

// ResponseHeaders implements envoy.HttpFilterInstance.
func (h *bodiesReplaceHttpFilterInstance) ResponseHeaders(headers envoy.ResponseHeaders, _ bool) envoy.ResponseHeadersStatus {
	append, ok := headers.Get("append")
	if ok {
		h.responseAppend = append.String()
	}
	prepend, ok := headers.Get("prepend")
	if ok {
		h.responsePrepend = prepend.String()
	}
	replace, ok := headers.Get("replace")
	if ok {
		h.responseReplace = replace.String()
	}
	headers.Remove("content-length") // Remove the content-length header to reset the length.
	return envoy.ResponseHeadersStatusContinue
}

// ResponseBody implements envoy.HttpFilterInstance.
func (h *bodiesReplaceHttpFilterInstance) ResponseBody(frame envoy.ResponseBodyBuffer, endOfStream bool) envoy.ResponseBodyStatus {
	fmt.Printf("new request body frame: %s\n", string(frame.Copy()))
	if !endOfStream {
		// Wait for the end of the stream to see the full body.
		return envoy.ResponseBodyStatusStopIterationAndBuffer
	}

	entireBody := h.envoyFilter.GetResponseBodyBuffer()
	if h.responseAppend != "" {
		entireBody.Append([]byte(h.responseAppend))
	}
	if h.responsePrepend != "" {
		entireBody.Prepend([]byte(h.responsePrepend))
	}
	if h.responseReplace != "" {
		entireBody.Replace([]byte(h.responseReplace))
	}
	return envoy.ResponseBodyStatusContinue
}

// Destroy implements envoy.HttpFilterInstance.
func (h *bodiesReplaceHttpFilterInstance) Destroy() {}
