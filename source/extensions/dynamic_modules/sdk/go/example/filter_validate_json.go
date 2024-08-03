package main

import (
	"encoding/json"

	"github.com/mathetake/envoy-dynamic-modules/source/extensions/dynmic_modules/sdk/go/envoy"
)

// validateJsonFilter implements envoy.HttpFilter.
type validateJsonFilter struct{}

func newValidateJsonFilter(string) envoy.HttpFilter { return &validateJsonFilter{} }

// NewInstance implements envoy.HttpFilter.
func (f *validateJsonFilter) NewInstance(envoyFilter envoy.EnvoyFilterInstance) envoy.HttpFilterInstance {
	return &validateJsonFilterInstance{envoyFilter: envoyFilter}
}

// Destroy implements envoy.HttpFilter.
func (f *validateJsonFilter) Destroy() {}

// validateJsonFilterInstance implements envoy.HttpFilterInstance.
type validateJsonFilterInstance struct {
	envoyFilter envoy.EnvoyFilterInstance
}

// RequestHeaders implements envoy.HttpFilterInstance.
func (h *validateJsonFilterInstance) RequestHeaders(envoy.RequestHeaders, bool) envoy.RequestHeadersStatus {
	return envoy.HeadersStatusContinue
}

type validateJsonFilterBody struct {
	Foo string `json:"foo"`
}

// RequestBody implements envoy.HttpFilterInstance.
func (h *validateJsonFilterInstance) RequestBody(frame envoy.RequestBodyBuffer, endOfStream bool) envoy.RequestBodyStatus {
	if !endOfStream {
		// Wait for the end of the stream to see the full body.
		return envoy.RequestBodyStatusStopIterationAndBuffer
	}
	entireBody := h.envoyFilter.GetRequestBodyBuffer()

	// This demonstrates how to get Reader from the buffer and read the entire body.
	reader := entireBody.NewReader()
	var b validateJsonFilterBody
	if err := json.NewDecoder(reader).Decode(&b); err != nil || b.Foo != "bar" {
		h.envoyFilter.SendResponse(400, nil, nil)
	}
	return envoy.RequestBodyStatusContinue
}

// ResponseHeaders implements envoy.HttpFilterInstance.
func (h *validateJsonFilterInstance) ResponseHeaders(envoy.ResponseHeaders, bool) envoy.ResponseHeadersStatus {
	return envoy.ResponseHeadersStatusContinue
}

// ResponseBody implements envoy.HttpFilterInstance.
func (h *validateJsonFilterInstance) ResponseBody(frame envoy.ResponseBodyBuffer, endOfStream bool) envoy.ResponseBodyStatus {
	return envoy.ResponseBodyStatusContinue
}

// Destroy implements envoy.HttpFilterInstance.
func (h *validateJsonFilterInstance) Destroy() {}
