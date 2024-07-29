package envoy

// This file corresponds to the enums defined in abi.h.

// RequestHeadersStatus is the return value of the HttpFilterInstance.EventHttpRequestHeaders event.
type RequestHeadersStatus int

const (
	// HeadersStatusContinue is returned when the operation should continue.
	HeadersStatusContinue RequestHeadersStatus = 0
	// RequestHeadersStatusStopIteration indicates that Envoy shouldn't continue
	// from processing the headers and should stop filter iteration. In other words, HttpFilterInstance.EventHttpRequestBody
	// will be called while not sending headers to the upstream. The header
	// processing can be resumed by either calling EnvoyFilterInstance.ContinueRequest, or returns
	// continue status from the HttpFilterInstance.EventHttpRequestBody.
	RequestHeadersStatusStopIteration RequestHeadersStatus = 1
	// RequestHeadersStatusStopAllIterationAndBuffer indicates
	// that Envoy should stop all iteration and continue to buffer the request body
	// until the limit is reached. When the limit is reached, Envoy will stop buffering and returns 500
	// to the client. This means that HttpFilterInstance.EventHttpRequestBody will not be called.
	//
	// The header processing can be resumed by either calling EnvoyFilterInstance.ContinueRequest, or
	// returns continue status from the HttpFilterInstance.EventHttpRequestBody.
	RequestHeadersStatusStopAllIterationAndBuffer RequestHeadersStatus = 3
)

// RequestBodyStatus is the return value of the HttpFilterInstance.EventHttpRequestBody event.
type RequestBodyStatus int

const (
	// RequestBodyStatusContinue is returned when the operation should continue.
	RequestBodyStatusContinue RequestBodyStatus = 0
	// RequestBodyStatusStopIterationAndBuffer indicates that Envoy shouldn't continue
	// from processing the body frame and should stop iteration, but continue buffering the body until
	// the limit is reached. When the limit is reached, Envoy will stop buffering and returns 500 to the
	// client.
	//
	// This stops sending body data to the upstream, so if the module wants to continue sending body
	// data, it should call EnvoyFilterInstance.ContinueRequest or return continue status in the
	// subsequent HttpFilterInstance.EventHttpRequestBody calls.
	RequestBodyStatusStopIterationAndBuffer RequestBodyStatus = 1
)

// ResponseHeadersStatus is the return value of the HttpFilterInstance.EventHttpResponseHeaders event.
type ResponseHeadersStatus int

const (
	// ResponseHeadersStatusContinue is returned when the operation should continue.
	ResponseHeadersStatusContinue ResponseHeadersStatus = 0
	// EventHttpResponseHeadersStatusStopIteration indicates that Envoy shouldn't continue
	// from processing the headers and should stop filter iteration. In other words, EventHttpResponseBody
	// will be called while not sending headers to the upstream. The header
	// processing can be resumed by either calling EnvoyFilterInstance.ContinueResponse, or returns
	// continue status from the EventHttpResponseBody.
	ResponseHeadersStatusStopIteration ResponseHeadersStatus = 1

	// ResponseHeadersStatusStopAllIterationAndBuffer indicates
	// that Envoy should stop all iteration and continue to buffer the response body
	// until the limit is reached. When the limit is reached, Envoy will stop buffering and returns 500
	// to the client. This means that HttpFilterInstance.EventHttpResponseBody will not be called.
	//
	// The header processing can be resumed by either calling EnvoyFilterInstance.ContinueResponse, or
	// returns continue status from the HttpFilterInstance.EventHttpResponseBody.
	ResponseHeadersStatusStopAllIterationAndBuffer ResponseHeadersStatus = 3
)

// ResponseBodyStatus is the return value of the HttpFilterInstance.EventHttpResponseBody event.
type ResponseBodyStatus int

const (
	// ResponseBodyStatusContinue is returned when the operation should continue.
	ResponseBodyStatusContinue ResponseBodyStatus = 0
	// ResponseBodyStatusStopIterationAndBuffer indicates that Envoy shouldn't continue
	// from processing the body frame and should stop iteration, but continue buffering the body until
	// the limit is reached. When the limit is reached, Envoy will stop buffering and returns 500 to the
	// client.
	//
	// This stops sending body data to the upstream, so if the module wants to continue sending body
	// data, it should call EnvoyFilterInstance.ContinueResponse or return continue status in the
	// subsequent HttpFilterInstance.EventHttpResponseBody calls.
	ResponseBodyStatusStopIterationAndBuffer ResponseBodyStatus = 1
)
