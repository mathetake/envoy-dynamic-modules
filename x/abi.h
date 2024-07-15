#pragma once

#include <cstdio>

namespace Envoy {
namespace Http {
namespace DynamicModule {
namespace ABI {

// -------------- Event Hooks --------------

// EnvoyFilterPtr is a pointer to the DynamicModule::HttpFilter instance. It is always passed to the
// module's event hooks. Modules are not supposed to manipulate this pointer.
using EnvoyFilterPtr = void*;

// ModuleContextPtr is a pointer to in-module context corresponding to a single
// DynamicModule::HttpFilter instance. It is always passed to the module's event hooks.
using ModuleContextPtr = void*;

// RequestHeadersPtr is a pointer to the RequestHeaderMap instance. This is passed to the
// __envoy_dynamic_module_http_on_request_headers event hook.
// Modules are not supposed to manipulate this pointer.
using RequestHeadersPtr = void*;

// OnRequestHeadersStatus is the return value of the __envoy_dynamic_module_http_on_request_headers
// event. It should be one of the values defined in the FilterHeadersStatus enum.
using OnRequestHeadersStatus = size_t;

// ResponseHeaderMapPtr is a pointer to the ResponseHeaderMap instance. This is passed to the
// __envoy_dynamic_module_http_on_response_headers event hook.
// Modules are not supposed to manipulate this pointer.
using ResponseHeaderMapPtr = void*;

// OnResponseHeadersStatus is the return value of the
// __envoy_dynamic_module_http_on_response_headers event. It should be one of the values defined in
// the FilterHeadersStatus enum.
using OnResponseHeadersStatus = size_t;

// RequestBufferPtr is a pointer to the Buffer::Instance instance. This is passed to the
// __envoy_dynamic_module_http_on_request_body event hook.
// Modules are not supposed to manipulate this pointer.
using RequestBufferPtr = void*;

// OnRequestBodyStatus is the return value of the __envoy_dynamic_module_http_on_request_body event.
// It should be one of the values defined in the FilterDataStatus enum.
using OnRequestBodyStatus = size_t;

// ResponseBufferPtr is a pointer to the Buffer::Instance instance. This is passed to the
// __envoy_dynamic_module_http_on_response_body event hook.
// Modules are not supposed to manipulate this pointer.
using ResponseBufferPtr = void*;

// OnResponseBodyStatus is the return value of the __envoy_dynamic_module_http_on_response_body
// event. It should be one of the values defined in the FilterDataStatus enum.
using OnResponseBodyStatus = size_t;

// __envoy_dynamic_module_init is called by the main thread when the module is loaded exactly once
// per module. The function should return 0 on success and non-zero on failure.
constexpr char __envoy_dynamic_module_init[] = "__envoy_dynamic_module_init";
using EnvoyModuleInitSig = size_t (*)(const char*);

// __envoy_dynamic_module_http_context_init is called by any worker thread when a new stream is
// created. That means that the function should be thread-safe.
//
// The function should return a pointer to a new instance of the context or nullptr on failure.
// The lifetime of the returned pointer should be managed by the dynamic module.
constexpr char __envoy_dynamic_module_http_context_init[] =
    "__envoy_dynamic_module_http_context_init";
using EnvoyModuleHttpContextInitSig = ModuleContextPtr (*)();

// __envoy_dynamic_module_http_on_request_headers is called when request headers are received.
constexpr char __envoy_dynamic_module_http_on_request_headers[] =
    "__envoy_dynamic_module_http_on_request_headers";
using EnvoyModuleHttpOnRequestHeadersSig = OnRequestHeadersStatus (*)(EnvoyFilterPtr,
                                                                      ModuleContextPtr,
                                                                      RequestHeadersPtr,
                                                                      bool /* End of stream */);

// __envoy_dynamic_module_http_on_request_body is called when request body data is received.
constexpr char __envoy_dynamic_module_http_on_request_body[] =
    "__envoy_dynamic_module_http_on_request_body";
using EnvoyModuleHttpOnRequestBodySig = OnRequestBodyStatus (*)(EnvoyFilterPtr, ModuleContextPtr,
                                                                RequestBufferPtr,
                                                                bool /* End of stream */);

// __envoy_dynamic_module_http_on_response_headers is called when response headers are received.
constexpr char __envoy_dynamic_module_http_on_response_headers[] =
    "__envoy_dynamic_module_http_on_response_headers";
using EnvoyModuleHttpOnResponseHeadersSig = OnResponseHeadersStatus (*)(EnvoyFilterPtr,
                                                                        ModuleContextPtr,
                                                                        ResponseHeaderMapPtr,
                                                                        bool /* End of stream */);

// __envoy_dynamic_module_http_on_response_body is called when response body data is received.
constexpr char __envoy_dynamic_module_http_on_response_body[] =
    "__envoy_dynamic_module_http_on_response_body";
using EnvoyModuleHttpOnResponseBodySig = OnResponseBodyStatus (*)(EnvoyFilterPtr, ModuleContextPtr,
                                                                  ResponseBufferPtr,
                                                                  bool /* End of stream */);

// __envoy_dynamic_module_http_on_destroy is called when the stream is destroyed.
constexpr char __envoy_dynamic_module_http_on_destroy[] = "__envoy_dynamic_module_http_on_destroy";
using EnvoyModuleHttpOnDestroySig = void (*)(EnvoyFilterPtr, ModuleContextPtr);

// -------------- Envoy Functions --------------

using InModuleBufferPtr = const char*;
using InModuleBufferLength = size_t;
using DataSlicePtr = const char*;
using DataSliceLength = size_t;

// The following functions are implemented by Envoy and are called by the module to interact with
// it.
extern "C" {

// __envoy_dynamic_module_get_request_header is called by the module to get the value for a request
// header key. headers is the one passed to the __envoy_dynamic_module_http_on_request_headers.
// key is the header key to look up. result_buffer_ptr and result_buffer_length_ptr are direct
// references to the buffer and length of the value. The function should return the number of
// values found. If the key is not found, this function should return nullptr and 0.
//
// Basically, this acts as a fast zero-copy lookup for a single header value, which is almost always
// guaranteed to be true. In case of multiple values, the module can access n-th value by calling
// __envoy_dynamic_module_http_get_request_header_value_nth following this function.
size_t __envoy_dynamic_module_http_get_request_header_value(
    ResponseHeaderMapPtr headers, InModuleBufferPtr key, InModuleBufferLength key_length,
    InModuleBufferPtr* result_buffer_ptr, InModuleBufferLength* result_buffer_length_ptr);

// __envoy_dynamic_module_http_get_request_header_value_nth is almost the same as
// __envoy_dynamic_module_http_get_request_header_value, but it allows the module to access n-th
// value of the header. The function should return the number of values found. If nth is out of
// bounds, this function returns nullptr and 0.
void __envoy_dynamic_module_http_get_request_header_value_nth(
    ResponseHeaderMapPtr headers, InModuleBufferPtr key, InModuleBufferLength key_length,
    InModuleBufferPtr* result_buffer_ptr, InModuleBufferLength* result_buffer_length_ptr,
    size_t nth);

// __envoy_dynamic_module_http_get_response_header_value is called by the module to get the value
// for a response header key. headers is the one passed to the
// __envoy_dynamic_module_http_on_response_headers. key is the header key to look up.
// result_buffer_ptr and result_buffer_length_ptr are direct references to the buffer and length of
// the value. The function should return the number of values found. If the key is not found, this
// function should return nullptr and 0.
//
// Basically, this acts as a fast zero-copy lookup for a single header value, which is almost always
// guaranteed to be true. In case of multiple values, the module can access n-th value by calling
// __envoy_dynamic_module_http_get_response_header_value_nth following this function.
size_t __envoy_dynamic_module_http_get_response_header_value(
    ResponseHeaderMapPtr headers, InModuleBufferPtr key, InModuleBufferLength key_length,
    InModuleBufferPtr* result_buffer_ptr, InModuleBufferLength* result_buffer_length_ptr);

// __envoy_dynamic_module_http_get_response_header_value_nth is almost the same as
// __envoy_dynamic_module_http_get_response_header_value, but it allows the module to access n-th
// value of the header. The function should return the number of values found. If nth is out of
// bounds, this function returns nullptr and 0.
void __envoy_dynamic_module_http_get_response_header_value_nth(
    ResponseHeaderMapPtr headers, InModuleBufferPtr key, InModuleBufferLength key_length,
    InModuleBufferPtr* result_buffer_ptr, InModuleBufferLength* result_buffer_length_ptr,
    size_t nth);

// __envoy_dynamic_module_http_get_request_body_buffer_slices_count is called by the module to get
// the number of slices in the request body buffer. The function should return the number of slices.
size_t __envoy_dynamic_module_http_get_request_body_buffer_slices_count(RequestBufferPtr buffer);

// __envoy_dynamic_module_http_get_request_body_buffer_slice is called by the module to get the n-th
// slice of the request body buffer. The function should return the buffer and length of the slice.
// If nth is out of bounds, this function returns nullptr and 0.
void __envoy_dynamic_module_http_get_request_body_buffer_slice(
    RequestBufferPtr buffer, size_t nth, DataSlicePtr* result_buffer_ptr,
    DataSliceLength* result_buffer_length_ptr);

// __envoy_dynamic_module_http_get_response_body_buffer_slices_count is called by the module to get
// the number of slices in the response body buffer. The function should return the number of
// slices.
size_t __envoy_dynamic_module_http_get_response_body_buffer_slices_count(ResponseBufferPtr buffer);

// __envoy_dynamic_module_http_get_response_body_buffer_slice is called by the module to get the
// n-th slice of the response body buffer. The function should return the buffer and length of the
// slice. If nth is out of bounds, this function returns nullptr and 0.
void __envoy_dynamic_module_http_get_response_body_buffer_slice(
    ResponseBufferPtr buffer, size_t nth, DataSlicePtr* result_buffer_ptr,
    DataSliceLength* result_buffer_length_ptr);

} // extern "C"

} // namespace ABI
} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
