#ifndef ENVOY_DYNAMIC_MODULE_ABI_
#define ENVOY_DYNAMIC_MODULE_ABI_

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
extern "C" {
#else
#include <stddef.h>
#include <stdint.h>
#endif

// -----------------------------------------------------------------------------
// ----------------------------------- Types -----------------------------------
// -----------------------------------------------------------------------------

// These two macros are used to indicate the ownership of the memory for the types suffixed by
// `Ptr`.
#define OWNED_BY_ENVOY  // Indicates that the memory is owned by Envoy.
#define OWNED_BY_MODULE // Indicates that the memory is owned by the module.

#ifdef __cplusplus
typedef void* envoy_dynamic_module_raw_pointer;
#else
// Use uintptr_t to represent a raw pointer in C for simplicity in Go bindings.
typedef uintptr_t envoy_dynamic_module_raw_pointer;
#endif

// envoy_dynamic_module_type_HttpFilterConfigPtr is a pointer to the configuration passed to
// the
// envoy_dynamic_module_on_http_filter_init function. Envoy owns the memory of the
// configuration and the module is not supposed to take ownership of it.
typedef envoy_dynamic_module_raw_pointer envoy_dynamic_module_type_HttpFilterConfigPtr
    OWNED_BY_ENVOY;

// envoy_dynamic_module_type_HttpFilterConfigSize is the size of the configuration passed to
// the
// envoy_dynamic_module_on_http_filter_init function.
typedef size_t envoy_dynamic_module_type_HttpFilterConfigSize;

// envoy_dynamic_module_type_HttpFilterPtr is a pointer to in-module singleton context
// corresponding to the module. This is passed to
// envoy_dynamic_module_on_http_filter_instance_init.
typedef envoy_dynamic_module_raw_pointer envoy_dynamic_module_type_HttpFilterPtr OWNED_BY_MODULE;

// envoy_dynamic_module_type_EnvoyFilterInstancePtr is a pointer to the
// DynamicModule::HttpFilter instance. Modules are not supposed to manipulate this pointer.
//
// This is passed to envoy_dynamic_module_on_http_filter_instance_init, and the context can
// store this pointer to access the filter instance. However, this becomes invalid after the
// envoy_dynamic_module_on_http_filter_instance_destroy is called.
typedef envoy_dynamic_module_raw_pointer envoy_dynamic_module_type_EnvoyFilterInstancePtr
    OWNED_BY_ENVOY;

// envoy_dynamic_module_type_HttpFilterInstancePtr is a pointer to in-module context
// corresponding to a single DynamicModule::HttpFilter instance. It is always passed to the module's
// event hooks.
typedef envoy_dynamic_module_raw_pointer envoy_dynamic_module_type_HttpFilterInstancePtr
    OWNED_BY_MODULE;

// envoy_dynamic_module_type_HttpRequestHeadersMapPtr is a pointer to the header map instance.
// This is passed to the envoy_dynamic_module_on_http_filter_instance_request_headers event
// hook. Modules are not supposed to manipulate this pointer.
typedef envoy_dynamic_module_raw_pointer envoy_dynamic_module_type_HttpRequestHeadersMapPtr
    OWNED_BY_ENVOY;

// envoy_dynamic_module_type_EventHttpRequestHeadersStatus is the return value of the
// envoy_dynamic_module_on_http_filter_instance_request_headers event. It should be one of
// the values defined in the FilterHeadersStatus enum.
typedef size_t envoy_dynamic_module_type_EventHttpRequestHeadersStatus;

// envoy_dynamic_module_type_HttpResponseHeaderMapPtr is a pointer to the header map instance.
// This is passed to the envoy_dynamic_module_on_http_filter_instance_response_headers event
// hook. Modules are not supposed to manipulate this pointer.
typedef envoy_dynamic_module_raw_pointer envoy_dynamic_module_type_HttpResponseHeaderMapPtr
    OWNED_BY_ENVOY;

// envoy_dynamic_module_type_EventHttpResponseHeadersStatus is the return value of the
// envoy_dynamic_module_on_http_filter_instance_response_headers event. It should be one of
// the values defined in the FilterHeadersStatus enum.
typedef size_t envoy_dynamic_module_type_EventHttpResponseHeadersStatus;

// envoy_dynamic_module_type_HttpRequestBodyBufferPtr is a pointer to the body buffer instance
// passed via envoy_dynamic_module_on_http_filter_instance_request_body event hook.
// Modules are not supposed to manipulate this pointer directly.
typedef envoy_dynamic_module_raw_pointer envoy_dynamic_module_type_HttpRequestBodyBufferPtr
    OWNED_BY_ENVOY;

// envoy_dynamic_module_type_EventHttpRequestBodyStatus is the return value of the
// envoy_dynamic_module_on_http_filter_instance_request_body event. It should be one of the
// values defined in the FilterDataStatus enum.
typedef size_t envoy_dynamic_module_type_EventHttpRequestBodyStatus;

// envoy_dynamic_module_type_HttpResponseBodyBufferPtr is a pointer to the body buffer instance
// passed via envoy_dynamic_module_on_http_filter_instance_response_body event hook.
// Modules are not supposed to manipulate this pointer directly.
typedef envoy_dynamic_module_raw_pointer envoy_dynamic_module_type_HttpResponseBodyBufferPtr
    OWNED_BY_ENVOY;

// envoy_dynamic_module_type_EventHttpResponseBodyStatus is the return value of the
// envoy_dynamic_module_on_http_filter_instance_response_body event. It should be one of the
// values defined in the FilterDataStatus enum.
typedef size_t envoy_dynamic_module_type_EventHttpResponseBodyStatus;

// envoy_dynamic_module_type_EndOfStream is a boolean value indicating whether the stream has
// reached the end. The value should be 0 if the stream has not reached the end, and 1 if the stream
// has reached the end.
typedef size_t envoy_dynamic_module_type_EndOfStream;

// envoy_dynamic_module_type_InModuleBufferPtr is a pointer to a buffer that is managed by the
// module.
typedef envoy_dynamic_module_raw_pointer envoy_dynamic_module_type_InModuleBufferPtr
    OWNED_BY_MODULE;

// envoy_dynamic_module_type_InModuleBufferLength is the length of the buffer.
typedef size_t envoy_dynamic_module_type_InModuleBufferLength;

// envoy_dynamic_module_type_DataSlicePtr is a pointer to a buffer that is managed by Envoy.
// This is used to pass buffer slices to the module.
typedef envoy_dynamic_module_raw_pointer envoy_dynamic_module_type_DataSlicePtr OWNED_BY_ENVOY;

// envoy_dynamic_module_type_DataSlicePtrResult is a pointer to a
// envoy_dynamic_module_type_DataSlicePtr that is managed by the module.
typedef envoy_dynamic_module_raw_pointer envoy_dynamic_module_type_DataSlicePtrResult
    OWNED_BY_MODULE;

// envoy_dynamic_module_type_DataSliceLength is the length of the buffer slice.
typedef size_t envoy_dynamic_module_type_DataSliceLength;

// envoy_dynamic_module_type_DataSliceLengthResult is a pointer to a
// envoy_dynamic_module_type_DataSliceLength that is managed by the module.
typedef size_t envoy_dynamic_module_type_DataSliceLengthResult;

// envoy_dynamic_module_type_InModuleHeader is a struct that contains representation of a
// header. This is used to pass headers to Envoy from modules.
//
// This matches the memory representation of `[2]string` in Go and `(&str, &str)` in Rust.
typedef struct {
  envoy_dynamic_module_type_InModuleBufferPtr header_key;
  envoy_dynamic_module_type_InModuleBufferLength header_key_length;
  envoy_dynamic_module_type_InModuleBufferPtr header_value;
  envoy_dynamic_module_type_InModuleBufferLength header_value_length;
} envoy_dynamic_module_type_InModuleHeader;

// envoy_dynamic_module_type_InModuleBufferVectorPtr is a pointer to a vector of
// envoy_dynamic_module_type_InModuleHeader. This is currently only used for sending local
// responses.
//
// This combined with envoy_dynamic_module_type_InModuleHeadersSize can be used to
// pass the data `[][2]string` in Go and `vec<(&str, &str)>` in Rust.
typedef envoy_dynamic_module_raw_pointer envoy_dynamic_module_type_InModuleHeadersPtr;
OWNED_BY_MODULE;

// envoy_dynamic_module_type_InModuleHeadersSize is the size of the vector of buffers.
typedef size_t envoy_dynamic_module_type_InModuleHeadersSize;

// -----------------------------------------------------------------------------
// ----------------------------------- Enums -----------------------------------
// -----------------------------------------------------------------------------

// ENVOY_DYNAMIC_MODULE_HEADER_STATUS_CONTINUE indicates that the module has finished
// processing the headers and Envoy should continue processing the request body or response body.
#define ENVOY_DYNAMIC_MODULE_HEADER_STATUS_CONTINUE 0
// ENVOY_DYNAMIC_MODULE_HEADER_STATUS_STOP_ITERATION indicates that Envoy shouldn't continue
// from processing the headers and should stop iteration. In other words, event_http_request_body
// and event_http_response_body will be called while not sending headers to the upstream. The header
// processing can be resumed by either calling continue_request/continue_response, or returns
// continue status in the event_http_request_body or event_http_response_body.
#define ENVOY_DYNAMIC_MODULE_HEADER_STATUS_STOP_ITERATION 1
// ENVOY_DYNAMIC_MODULE_HEADER_STATUS_STOP_ALL_ITERATION_AND_BUFFER indicates
// that Envoy should stop all iteration and continue to buffer the request/response body
// until the limit is reached. When the limit is reached, Envoy will stop buffering and returns 500
// to the client. This means that event_http_request_body and event_http_response_body will not be
// called.
//
// The header processing can be resumed by either calling continue_request/continue_response, or
// returns continue status in the event_http_request_body or event_http_response_body.
#define ENVOY_DYNAMIC_MODULE_HEADER_STATUS_STOP_ALL_ITERATION_AND_BUFFER 3

static const envoy_dynamic_module_type_EventHttpRequestHeadersStatus
    envoy_dynamic_module_type_EventHttpRequestHeadersStatusContinue =
        ENVOY_DYNAMIC_MODULE_HEADER_STATUS_CONTINUE;
static const envoy_dynamic_module_type_EventHttpRequestHeadersStatus
    envoy_dynamic_module_type_EventHttpRequestHeadersStatusStopIteration =
        ENVOY_DYNAMIC_MODULE_HEADER_STATUS_STOP_ITERATION;
static const envoy_dynamic_module_type_EventHttpRequestHeadersStatus
    envoy_dynamic_module_type_EventHttpRequestHeadersStatusStopAllIterationAndBuffer =
        ENVOY_DYNAMIC_MODULE_HEADER_STATUS_STOP_ALL_ITERATION_AND_BUFFER;

static const envoy_dynamic_module_type_EventHttpResponseHeadersStatus
    envoy_dynamic_module_type_EventHttpResponseHeadersStatusContinue =
        ENVOY_DYNAMIC_MODULE_HEADER_STATUS_CONTINUE;
static const envoy_dynamic_module_type_EventHttpResponseHeadersStatus
    envoy_dynamic_module_type_EventHttpResponseHeadersStatusStopIteration =
        ENVOY_DYNAMIC_MODULE_HEADER_STATUS_STOP_ITERATION;
static const envoy_dynamic_module_type_EventHttpResponseHeadersStatus
    envoy_dynamic_module_type_EventHttpResponseHeadersStatusStopAllIterationAndBuffer =
        ENVOY_DYNAMIC_MODULE_HEADER_STATUS_STOP_ALL_ITERATION_AND_BUFFER;

// ENVOY_DYNAMIC_MODULE_BODY_STATUS_CONTINUE indicates that the module has finished
// processing the body frame and Envoy should continue processing the request or response.
//
// This resumes the header processing if it was stopped in the event_http_request_headers or
// event_http_response_headers.
#define ENVOY_DYNAMIC_MODULE_BODY_STATUS_CONTINUE 0

// ENVOY_DYNAMIC_MODULE_BODY_STATUS_STOP_ITERATION indicates that Envoy shouldn't continue
// from processing the body frame and should stop iteration, but continue buffering the body until
// the limit is reached. When the limit is reached, Envoy will stop buffering and returns 500 to the
// client.
//
// This stops sending body data to the upstream, so if the module wants to continue sending body
// data, it should call continue_request or continue_response or return continue status in the
// subsequent event_http_request_body or event_http_response_body.
#define ENVOY_DYNAMIC_MODULE_BODY_STATUS_STOP_ITERATION_AND_BUFFER 1

static const envoy_dynamic_module_type_EventHttpRequestBodyStatus
    envoy_dynamic_module_type_EventHttpRequestBodyStatusContinue =
        ENVOY_DYNAMIC_MODULE_BODY_STATUS_CONTINUE;
static const envoy_dynamic_module_type_EventHttpRequestBodyStatus
    envoy_dynamic_module_type_EventHttpRequestBodyStatusStopIterationAndBuffer =
        ENVOY_DYNAMIC_MODULE_BODY_STATUS_STOP_ITERATION_AND_BUFFER;

static const envoy_dynamic_module_type_EventHttpResponseBodyStatus
    envoy_dynamic_module_type_EventHttpResponseBodyStatusContinue =
        ENVOY_DYNAMIC_MODULE_BODY_STATUS_CONTINUE;
static const envoy_dynamic_module_type_EventHttpResponseBodyStatus
    envoy_dynamic_module_type_EventHttpResponseBodyStatusStopIterationAndBuffer =
        ENVOY_DYNAMIC_MODULE_BODY_STATUS_STOP_ITERATION_AND_BUFFER;

// -----------------------------------------------------------------------------
// ------------------------------- Event Hooks ---------------------------------
// -----------------------------------------------------------------------------
//
// Event hooks are functions that are called by Envoy to notify the module of events.
// The module must implement and export these functions in the dynamic module.

// envoy_dynamic_module_on_program_init is called by the main thread when the module is
// loaded exactly once per shared object file. The function returns 0 on success and non-zero on
// failure.
size_t envoy_dynamic_module_on_program_init();

// envoy_dynamic_module_on_http_filter_init is called by the main thread when the http
// filter is loaded. The function returns
// envoy_dynamic_module_type_HttpFilterPtr which is a pointer to the in-module singleton
// context per http filter configuration. The lifetime of the returned pointer should be managed by
// the dynamic module. Returning nullptr indicates a failure to initialize the module.
envoy_dynamic_module_type_HttpFilterPtr envoy_dynamic_module_on_http_filter_init(
    envoy_dynamic_module_type_HttpFilterConfigPtr config_ptr,
    envoy_dynamic_module_type_HttpFilterConfigSize config_size);

// envoy_dynamic_module_on_http_filter_destroy is called exactly once when the http
// filter is unloaded. The function should clean up the resources allocated by the f
void envoy_dynamic_module_on_http_filter_destroy(
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr);

// envoy_dynamic_module_on_http_filter_instance_init is called by any worker thread when a
// new stream is created. That means that the function should be thread-safe.
//
// The function returns a pointer to a new instance of the context or nullptr on failure.
// The lifetime of the returned pointer should be managed by the dynamic module.
envoy_dynamic_module_type_HttpFilterInstancePtr envoy_dynamic_module_on_http_filter_instance_init(
    envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr);

// envoy_dynamic_module_on_http_filter_instance_request_headers is called when request
// headers are received.
envoy_dynamic_module_type_EventHttpRequestHeadersStatus
envoy_dynamic_module_on_http_filter_instance_request_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestHeadersMapPtr request_headers_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream);

// envoy_dynamic_module_on_http_filter_instance_request_body is called when request body
// data is received. buffer only contains the data for the current event.
envoy_dynamic_module_type_EventHttpRequestBodyStatus
envoy_dynamic_module_on_http_filter_instance_request_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream);

// envoy_dynamic_module_on_http_filter_instance_response_headers is called when response
// headers are received.
envoy_dynamic_module_type_EventHttpResponseHeadersStatus
envoy_dynamic_module_on_http_filter_instance_response_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream);

// envoy_dynamic_module_on_http_filter_instance_response_body is called when response body
// data is received. buffer only contains the data for the current event.
envoy_dynamic_module_type_EventHttpResponseBodyStatus
envoy_dynamic_module_on_http_filter_instance_response_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream);

// envoy_dynamic_module_on_http_filter_instance_destroy is called when the stream is
// destroyed.
void envoy_dynamic_module_on_http_filter_instance_destroy(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr);

#undef OWNED_BY_ENVOY
#undef OWNED_BY_MODULE

// ---------------------------------------------------------------------------------
// ----------------------------------- Envoy API -----------------------------------
// ---------------------------------------------------------------------------------
//
// The following functions are called by the module to interact with Envoy.
//
// Note that pointers owned by Envoy should be made sure valid by following the caveat in the
// comments.

// ---------------- Header API ----------------

// envoy_dynamic_module_get_request_header is called by the module to get the value for a
// request header key. headers is the one passed to the
// envoy_dynamic_module_on_http_filter_instance_request_headers. key is the header key to
// look up. result_buffer_ptr and result_buffer_length_ptr are direct references to the buffer and
// length of the value. The function returns the number of values found. If the key is not found,
// this function returns nullptr and 0.
//
// Basically, this acts as a fast zero-copy lookup for a single header value, which is almost always
// guaranteed to be true. In case of multiple values, the module can access n-th value by calling
// envoy_dynamic_module_http_get_request_header_value_nth following this function.
size_t envoy_dynamic_module_http_get_request_header_value(
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers,
    envoy_dynamic_module_type_InModuleBufferPtr key,
    envoy_dynamic_module_type_InModuleBufferLength key_length,
    envoy_dynamic_module_type_DataSlicePtrResult result_buffer_ptr,
    envoy_dynamic_module_type_DataSliceLengthResult result_buffer_length_ptr);

// envoy_dynamic_module_http_get_request_header_value_nth is almost the same as
// envoy_dynamic_module_http_get_request_header_value, but it allows the module to access n-th
// value of the header. The function returns the number of values found. If nth is out of
// bounds, this function returns nullptr and 0.
void envoy_dynamic_module_http_get_request_header_value_nth(
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers,
    envoy_dynamic_module_type_InModuleBufferPtr key,
    envoy_dynamic_module_type_InModuleBufferLength key_length,
    envoy_dynamic_module_type_DataSlicePtrResult result_buffer_ptr,
    envoy_dynamic_module_type_DataSliceLengthResult result_buffer_length_ptr, size_t nth);

// envoy_dynamic_module_http_get_response_header_value is called by the module to get the value
// for a response header key. headers is the one passed to the
// envoy_dynamic_module_on_http_filter_instance_response_headers. key is the header key to
// look up. result_buffer_ptr and result_buffer_length_ptr are direct references to the buffer and
// length of the value. The function returns the number of values found. If the key is not found,
// this function returns nullptr and 0.
//
// Basically, this acts as a fast zero-copy lookup for a single header value, which is almost always
// guaranteed to be true. In case of multiple values, the module can access n-th value by calling
// envoy_dynamic_module_http_get_response_header_value_nth following this function.
size_t envoy_dynamic_module_http_get_response_header_value(
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers,
    envoy_dynamic_module_type_InModuleBufferPtr key,
    envoy_dynamic_module_type_InModuleBufferLength key_length,
    envoy_dynamic_module_type_DataSlicePtrResult result_buffer_ptr,
    envoy_dynamic_module_type_DataSliceLengthResult result_buffer_length_ptr);

// envoy_dynamic_module_http_get_response_header_value_nth is almost the same as
// envoy_dynamic_module_http_get_response_header_value, but it allows the module to access n-th
// value of the header. The function returns the number of values found. If nth is out of
// bounds, this function returns nullptr and 0.
void envoy_dynamic_module_http_get_response_header_value_nth(
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers,
    envoy_dynamic_module_type_InModuleBufferPtr key,
    envoy_dynamic_module_type_InModuleBufferLength key_length,
    envoy_dynamic_module_type_DataSlicePtrResult result_buffer_ptr,
    envoy_dynamic_module_type_DataSliceLengthResult result_buffer_length_ptr, size_t nth);

// envoy_dynamic_module_http_set_request_header is called by the module to set the value
// for a request header key. headers is the one passed to the
// envoy_dynamic_module_on_http_filter_instance_request_headers. If the key is not found,
// this function should add a new header with the key and value. If the key is found, this function
// should replace the value with the new one. If the value is empty, this function should remove the
// key. If there are multiple headers with the same key, this function removes all of them and adds
// a new one.
void envoy_dynamic_module_http_set_request_header(
    envoy_dynamic_module_type_HttpRequestHeadersMapPtr headers,
    envoy_dynamic_module_type_InModuleBufferPtr key,
    envoy_dynamic_module_type_InModuleBufferLength key_length,
    envoy_dynamic_module_type_InModuleBufferPtr value,
    envoy_dynamic_module_type_InModuleBufferLength value_length);

// envoy_dynamic_module_http_set_response_header is called by the module to set the value
// for a response header key. headers is the one passed to the
// envoy_dynamic_module_on_http_filter_instance_response_headers.
// If the key is not found, this function should add a new header with the key and value. If the key
// is found, this function should replace the value with the new one. If the value is empty, this
// function should remove the key. If there are multiple headers with the same key, this function
// removes all of them and adds a new one.
void envoy_dynamic_module_http_set_response_header(
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers,
    envoy_dynamic_module_type_InModuleBufferPtr key,
    envoy_dynamic_module_type_InModuleBufferLength key_length,
    envoy_dynamic_module_type_InModuleBufferPtr value,
    envoy_dynamic_module_type_InModuleBufferLength value_length);

// ---------------- Buffer API ----------------

// envoy_dynamic_module_http_get_request_body_buffer is called by the module to get the entire
// request body buffer. The function returns the buffer if available, otherwise nullptr.
envoy_dynamic_module_type_HttpRequestBodyBufferPtr
envoy_dynamic_module_http_get_request_body_buffer(
    envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr);

// envoy_dynamic_module_http_get_response_body_buffer is called by the module to get the entire
// response body buffer. The function returns the buffer if available, otherwise nullptr.
envoy_dynamic_module_type_HttpResponseBodyBufferPtr
envoy_dynamic_module_http_get_response_body_buffer(
    envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr);

// envoy_dynamic_module_http_get_request_body_buffer_length is called by the module to get the
// length (number of bytes) of the request body buffer. The function returns the length of the
// buffer.
size_t envoy_dynamic_module_http_get_request_body_buffer_length(
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer);

// envoy_dynamic_module_http_get_request_body_buffer_slices_count is called by the module to
// get the number of slices in the request body buffer. The function returns the number of
// slices.
size_t envoy_dynamic_module_http_get_request_body_buffer_slices_count(
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer);

// envoy_dynamic_module_http_get_request_body_buffer_slice is called by the module to get the
// n-th slice of the request body buffer. The function returns the buffer and length of the
// slice. If nth is out of bounds, this function returns nullptr and 0.
void envoy_dynamic_module_http_get_request_body_buffer_slice(
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer, size_t nth,
    envoy_dynamic_module_type_DataSlicePtrResult result_buffer_ptr,
    envoy_dynamic_module_type_DataSliceLengthResult result_buffer_length_ptr);

// envoy_dynamic_module_http_copy_out_request_body_buffer is called by the module to copy
// `length` bytes from the request body buffer starting from `offset` to the `result_buffer_ptr`.
void envoy_dynamic_module_http_copy_out_request_body_buffer(
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer, size_t offset, size_t length,
    envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr);

// envoy_dynamic_module_http_append_request_body_buffer is called by the module to append
// data to the request body buffer. The function appends data to the end of the buffer.
//
// After calling this function, the previously returned slices may be invalidated.
void envoy_dynamic_module_http_append_request_body_buffer(
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer,
    envoy_dynamic_module_type_InModuleBufferPtr data,
    envoy_dynamic_module_type_InModuleBufferLength data_length);

// envoy_dynamic_module_http_prepend_request_body_buffer is called by the module to prepend
// data to the request body buffer. The function prepends data to the beginning of the buffer.
//
// After calling this function, the previously returned slices may be invalidated.
void envoy_dynamic_module_http_prepend_request_body_buffer(
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer,
    envoy_dynamic_module_type_InModuleBufferPtr data,
    envoy_dynamic_module_type_InModuleBufferLength data_length);

// envoy_dynamic_module_http_drain_request_body_buffer is called by the module to drain
// data from the request body buffer. The function drains length bytes from the beginning of the
// buffer.
//
// After calling this function, the previously returned slices may be invalidated.
void envoy_dynamic_module_http_drain_request_body_buffer(
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer, size_t length);

// envoy_dynamic_module_http_get_response_body_buffer_length is called by the module to get the
// length (number of bytes) of the response body buffer. The function returns the length of the
// buffer.
size_t envoy_dynamic_module_http_get_response_body_buffer_length(
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer);

// envoy_dynamic_module_http_get_response_body_buffer_slices_count is called by the module to
// get the number of slices in the response body buffer. The function returns the number of
// slices.
size_t envoy_dynamic_module_http_get_response_body_buffer_slices_count(
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer);

// envoy_dynamic_module_http_get_response_body_buffer_slice is called by the module to get the
// n-th slice of the response body buffer. The function returns the buffer and length of the
// slice. If nth is out of bounds, this function returns nullptr and 0.
void envoy_dynamic_module_http_get_response_body_buffer_slice(
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer, size_t nth,
    envoy_dynamic_module_type_DataSlicePtrResult result_buffer_ptr,
    envoy_dynamic_module_type_DataSliceLengthResult result_buffer_length_ptr);

// envoy_dynamic_module_http_copy_out_response_body_buffer is called by the module to copy
// `length` bytes from the response body buffer starting from `offset` to the `result_buffer_ptr`.
void envoy_dynamic_module_http_copy_out_response_body_buffer(
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer, size_t offset, size_t length,
    envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr);

// envoy_dynamic_module_http_append_response_body_buffer is called by the module to append
// data to the response body buffer. The function appends data to the end of the buffer.
//
// After calling this function, the previously returned slices may be invalidated.
void envoy_dynamic_module_http_append_response_body_buffer(
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer,
    envoy_dynamic_module_type_InModuleBufferPtr data,
    envoy_dynamic_module_type_InModuleBufferLength data_length);

// envoy_dynamic_module_http_prepend_response_body_buffer is called by the module to
// prepend data to the response body buffer. The function prepends data to the beginning of the
// buffer.
//
// After calling this function, the previously returned slices may be invalidated.
void envoy_dynamic_module_http_prepend_response_body_buffer(
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer,
    envoy_dynamic_module_type_InModuleBufferPtr data,
    envoy_dynamic_module_type_InModuleBufferLength data_length);

// envoy_dynamic_module_http_drain_response_body_buffer is called by the module to drain
// data from the response body buffer. The function drains length bytes from the beginning of the
// buffer.
//
// After calling this function, the previously returned slices may be invalidated.
void envoy_dynamic_module_http_drain_response_body_buffer(
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer, size_t length);

// envoy_dynamic_module_http_continue_request is called by the module to continue processing
// the request. This function is used when the module returned non Continue status in the events.
void envoy_dynamic_module_http_continue_request(
    envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr);

// envoy_dynamic_module_http_continue_response is called by the module to continue processing
// the response. This function is used when the module returned non Continue status in the events.
void envoy_dynamic_module_http_continue_response(
    envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr);

// ---------------- Miscellaneous API ----------------

// envoy_dynamic_module_http_send_response is called by the module to send a response to the
// client. headers_vector is a vector of headers to send. status_code is the status code to send.
// body is the body to send. body_length is the length of the body.
void envoy_dynamic_module_http_send_response(
    envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    uint32_t status_code, envoy_dynamic_module_type_InModuleHeadersPtr headers_vector,
    envoy_dynamic_module_type_InModuleHeadersSize headers_vector_size,
    envoy_dynamic_module_type_InModuleBufferPtr body,
    envoy_dynamic_module_type_InModuleBufferLength body_length);

#ifdef __cplusplus
}
#endif

#endif // ENVOY_DYNAMIC_MODULE_ABI_
