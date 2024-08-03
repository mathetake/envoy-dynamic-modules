// Package envoy provides the Go API for the Envoy filter chains.
package envoy

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
	RequestHeaders(requestHeaders RequestHeaders, endOfStream bool) RequestHeadersStatus
	// RequestBody is called when request body data is received.
	// The function should return the status of the operation.
	//
	//  * `frame` is the pointer to the newly arrived request body frame.
	//  * `endOfStream` is a boolean that indicates if this is the last data frame.
	RequestBody(frame RequestBodyBuffer, endOfStream bool) RequestBodyStatus
	// ResponseHeaders is called when response headers are received.
	// The function should return the status of the operation.
	//
	//  * `responseHeaders` is the pointer to the response headers map.
	//  * `endOfStream` is a boolean that indicates if this is the headers-only response.
	ResponseHeaders(responseHeaders ResponseHeaders, endOfStream bool) ResponseHeadersStatus
	// ResponseBody is called when response body data is received.
	// The function should return the status of the operation.
	//
	//  * `frame` is the pointer to the newly arrived response body frame.
	//  * `endOfStream` is a boolean that indicates if this is the last data frame.
	ResponseBody(frame ResponseBodyBuffer, endOfStream bool) ResponseBodyStatus

	// Destroy is called when the stream is destroyed.
	// This is called when the stream is completed or when the stream is reset.
	Destroy()
}
