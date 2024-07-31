package main

import (
	"fmt"
	"log"

	"github.com/mathetake/envoy-dynamic-modules/sdks/go/envoy"
)

// headersHttpFilter implements envoy.HttpFilter.
//
// This is to demonstrate how to use header manipulation APIs.
type headersHttpFilter struct{}

func newHeadersHttpFilter(string) envoy.HttpFilter { return &headersHttpFilter{} }

// NewInstance implements envoy.HttpFilter.
func (f *headersHttpFilter) NewInstance(envoy.EnvoyFilterInstance) envoy.HttpFilterInstance {
	return &headersHttpFilterInstance{}
}

// Destroy implements envoy.HttpFilter.
func (f *headersHttpFilter) Destroy() {}

// headersHttpFilterInstance implements envoy.HttpFilterInstance.
type headersHttpFilterInstance struct{}

// RequestHeaders implements envoy.HttpFilterInstance.
func (h *headersHttpFilterInstance) RequestHeaders(headers envoy.RequestHeaders, _ bool) envoy.RequestHeadersStatus {
	fooValue, _ := headers.Get("foo")
	if !fooValue.Equal("value") {
		log.Fatalf("expected foo to be \"value\", got %s", fooValue.String())
	}
	fmt.Println("foo:", fooValue.String())
	headers.Values("multiple-values", func(value envoy.HeaderValue) { fmt.Println("multiple-values:", value.String()) })
	headers.Remove("multiple-values")
	headers.Set("foo", "yes")
	headers.Set("multiple-values-to-be-single", "single")
	return envoy.HeadersStatusContinue
}

// RequestBody implements envoy.HttpFilterInstance.
func (h *headersHttpFilterInstance) RequestBody(envoy.RequestBodyBuffer, bool) envoy.RequestBodyStatus {
	return envoy.RequestBodyStatusContinue
}

// ResponseHeaders implements envoy.HttpFilterInstance.
func (h *headersHttpFilterInstance) ResponseHeaders(headers envoy.ResponseHeaders, _ bool) envoy.ResponseHeadersStatus {
	headers.Values("this-is", func(value envoy.HeaderValue) {
		if !value.Equal("response-header") {
			log.Fatalf("expected this-is to be \"response-header\", got %s", value.String())
		}
		fmt.Println("this-is:", value.String())
	})
	headers.Values("this-is-2", func(value envoy.HeaderValue) { fmt.Println("this-is-2:", value.String()) })

	headers.Set("this-is", "response-header")
	headers.Remove("this-is-2")
	headers.Set("multiple-values-res-to-be-single", "single")
	return envoy.ResponseHeadersStatusContinue
}

// ResponseBody implements envoy.HttpFilterInstance.
func (h *headersHttpFilterInstance) ResponseBody(envoy.ResponseBodyBuffer, bool) envoy.ResponseBodyStatus {
	return envoy.ResponseBodyStatusContinue
}

// Destroy implements envoy.HttpFilterInstance.
func (h *headersHttpFilterInstance) Destroy() {}
