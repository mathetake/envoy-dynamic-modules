package main

import (
	"net/http"

	"github.com/mathetake/envoy-dynamic-modules-go-sdk/envoy"
)

// sendResponseFilter implements envoy.HttpFilter.
//
// This is to demonstrate how to use body manipulation APIs.
type sendResponseFilter struct{}

func newSendResponseFilter(string) envoy.HttpFilter { return &sendResponseFilter{} }

// NewInstance implements envoy.HttpFilter.
func (f *sendResponseFilter) NewInstance(envoyFilter envoy.EnvoyFilterInstance) envoy.HttpFilterInstance {
	return &sendResponseFilterInstance{envoyFilter: envoyFilter}
}

// Destroy implements envoy.HttpFilter.
func (f *sendResponseFilter) Destroy() {}

// sendResponseFilterInstance implements envoy.HttpFilterInstance.
type sendResponseFilterInstance struct {
	envoyFilter envoy.EnvoyFilterInstance
	onResponse  bool
}

// RequestHeaders implements envoy.HttpFilterInstance.
func (h *sendResponseFilterInstance) RequestHeaders(headers envoy.RequestHeaders, _ bool) envoy.RequestHeadersStatus {
	if v, _ := headers.Get(":path"); v.Equal("/on_request") {
		h.envoyFilter.SendResponse(http.StatusUnauthorized, [][2]string{{"foo", "bar"}, {"bar", "baz"}}, []byte("local response at request headers"))
	}
	if v, _ := headers.Get(":path"); v.Equal("/on_response") {
		h.onResponse = true
	}
	return envoy.HeadersStatusContinue
}

// RequestBody implements envoy.HttpFilterInstance.
func (h *sendResponseFilterInstance) RequestBody(body envoy.RequestBodyBuffer, endOfStream bool) envoy.RequestBodyStatus {
	return envoy.RequestBodyStatusContinue
}

// ResponseHeaders implements envoy.HttpFilterInstance.
func (h *sendResponseFilterInstance) ResponseHeaders(headers envoy.ResponseHeaders, _ bool) envoy.ResponseHeadersStatus {
	if h.onResponse {
		h.envoyFilter.SendResponse(http.StatusInternalServerError, [][2]string{{"dog", "cat"}}, []byte("local response at response headers"))
	}
	return envoy.ResponseHeadersStatusContinue
}

// ResponseBody implements envoy.HttpFilterInstance.
func (h *sendResponseFilterInstance) ResponseBody(body envoy.ResponseBodyBuffer, endOfStream bool) envoy.ResponseBodyStatus {
	return envoy.ResponseBodyStatusContinue
}

// Destroy implements envoy.HttpFilterInstance.
func (h *sendResponseFilterInstance) Destroy() {}
