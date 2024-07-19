#include <stdio.h>
#include "abi.h"

void* __envoy_dynamic_module_v1_event_http_context_init() {
  static size_t context = 999999;
  return &context;
}

size_t __envoy_dynamic_module_v1_event_module_init(
    __envoy_dynamic_module_v1_type_ModuleConfigPtr config_ptr,
    __envoy_dynamic_module_v1_type_ModuleConfigSize config_size) {
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatus
__envoy_dynamic_module_v1_event_http_request_headers(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestHeadersMapPtr request_headers_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatus
__envoy_dynamic_module_v1_event_http_response_headers(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus
__envoy_dynamic_module_v1_event_http_request_body(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus
__envoy_dynamic_module_v1_event_http_response_body(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

void __envoy_dynamic_module_v1_event_http_destroy(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr) {}
