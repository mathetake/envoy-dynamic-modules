package main

import (
	"fmt"

	"github.com/mathetake/envoy-dynamic-modules/source/extensions/dynmic_modules/sdks/go/envoy"
)

// helloWorldFilter implements envoy.HttpFilter.
//
// This is to demonstrate how to create a simple http filter.
type helloWorldFilter struct{}

func newHelloWorldHttpFilter(string) envoy.HttpFilter { return &helloWorldFilter{} }

// NewInstance implements envoy.HttpFilter.
func (f *helloWorldFilter) NewInstance(envoy.EnvoyFilterInstance) envoy.HttpFilterInstance {
	fmt.Println("helloWorldHttpFilter.NewHttpFilterInstance called")
	return &helloWorldHttpFilterInstance{}
}

// Destroy implements envoy.HttpFilter.
func (f *helloWorldFilter) Destroy() { fmt.Println("helloWorldFilter.Destroy called") }

// helloWorldHttpFilterInstance implements envoy.HttpFilterInstance.
type helloWorldHttpFilterInstance struct{}

// RequestHeaders implements envoy.HttpFilterInstance.
func (h *helloWorldHttpFilterInstance) RequestHeaders(envoy.RequestHeaders, bool) envoy.RequestHeadersStatus {
	fmt.Println("helloWorldHttpFilterInstance.EventHttpRequestHeaders called")
	return envoy.HeadersStatusContinue
}

// RequestBody implements envoy.HttpFilterInstance.
func (h *helloWorldHttpFilterInstance) RequestBody(envoy.RequestBodyBuffer, bool) envoy.RequestBodyStatus {
	fmt.Println("helloWorldHttpFilterInstance.EventHttpRequestBody called")
	return envoy.RequestBodyStatusContinue
}

// ResponseHeaders implements envoy.HttpFilterInstance.
func (h *helloWorldHttpFilterInstance) ResponseHeaders(envoy.ResponseHeaders, bool) envoy.ResponseHeadersStatus {
	fmt.Println("helloWorldHttpFilterInstance.EventHttpResponseHeaders called")
	return envoy.ResponseHeadersStatusContinue
}

// ResponseBody implements envoy.HttpFilterInstance.
func (h *helloWorldHttpFilterInstance) ResponseBody(envoy.ResponseBodyBuffer, bool) envoy.ResponseBodyStatus {
	fmt.Println("helloWorldHttpFilterInstance.EventHttpResponseBody called")
	return envoy.ResponseBodyStatusContinue
}

// Destroy implements envoy.HttpFilterInstance.
func (h *helloWorldHttpFilterInstance) Destroy() {
	fmt.Println("helloWorldHttpFilterInstance.EventHttpDestroy called")
}
