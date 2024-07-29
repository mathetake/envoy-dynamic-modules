// Package envoy provides the Go API for the Envoy filter chains.
package envoy

import (
	"unsafe"
)

// NewHttpFilter is a function that creates a new HttpFilter that corresponds to each filter configuration in the Envoy filter chain.
// This is a global variable that should be set in the init function in the program once.
//
// The function is called once globally. The function is only called by the main thread,
// so it does not need to be thread-safe.
//
// `config` is the configuration string that is passed to the module that is set in the Envoy configuration.
var NewHttpFilter func(config string) HttpFilter

// HttpFilter is an interface that represents a single http filter in the Envoy filter chain.
// It is used to create HttpFilterInstance(s) that correspond to each Http request.
//
// This is only created once per module instance via the NewHttpFilter function.
type HttpFilter interface {
	// NewInstance is called for each new Http request.
	// Note that this must be concurrency-safe as it can be called concurrently for multiple requests.
	//
	// * `EnvoyFilterInstance` is the Envoy filter object that is used to interact with the underlying Envoy filter.
	//  This object is unique for each Http request. The object is destroyed when the stream is destroyed.
	//  Therefore, after EventHttpDestroy is called, this object should not be used.
	NewInstance(EnvoyFilterInstance) HttpFilterInstance

	// Destroy is called when this filter is destroyed. E.g. the filter chain configuration is updated and removed from the Envoy.
	Destroy()
}

// HttpFilterInstance is an interface that represents each Http request.
//
// Thisis created for each new Http request and is destroyed when the request is completed.
type HttpFilterInstance interface {
	// RequestHeaders is called when request headers are received.
	// The function should return the status of the operation.
	//
	//  * `requestHeaders` is the pointer to the request headers map.
	//  * `endOfStream` is a boolean that indicates if this is the headers-only request.
	RequestHeaders(RequestHeaders, bool) RequestHeadersStatus
	// RequestBody is called when request body data is received.
	// The function should return the status of the operation.
	//
	//  * `requestBody` is the pointer to the newly arrived request body buffer.
	//  * `endOfStream` is a boolean that indicates if this is the last data frame.
	RequestBody(RequestBodyBuffer, bool) RequestBodyStatus
	// ResponseHeaders is called when response headers are received.
	// The function should return the status of the operation.
	//
	//  * `responseHeaders` is the pointer to the response headers map.
	//  * `endOfStream` is a boolean that indicates if this is the headers-only response.
	ResponseHeaders(ResponseHeaders, bool) ResponseHeadersStatus
	// ResponseBody is called when response body data is received.
	// The function should return the status of the operation.
	//
	//  * `responseBody` is the pointer to the newly arrived response body buffer.
	//  * `endOfStream` is a boolean that indicates if this is the last data frame.
	ResponseBody(ResponseBodyBuffer, bool) ResponseBodyStatus

	// Destroy is called when the stream is destroyed.
	// This is called when the stream is completed or when the stream is reset.
	Destroy()
}

// HeaderValue represents a single header value whose data is owned by the Envoy.
//
// This is a view of the underlying data and doesn't copy the data.
type HeaderValue struct {
	data *byte
	size int
}

// String returns the string representation of the header value.
// This copies the underlying data to a new buffer and returns the string.
func (h HeaderValue) String() string {
	view := unsafe.Slice(h.data, h.size)
	return string(view)
}

// Equal returns true if the header value is equal to the given string.
//
// This doesn't copy the data and compares the data directly.
func (h HeaderValue) Equal(str string) bool {
	if h.size != len(str) || h.data == nil {
		return false
	}
	v := unsafe.String(h.data, h.size)
	return v == str
}