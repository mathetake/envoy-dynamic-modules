#include <stdio.h>
#include "abi.h"

__envoy_dynamic_module_v1_type_HttpFilterInstancePtr
__envoy_dynamic_module_v1_event_http_filter_instance_init(
    __envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpFilterPtr http_filter_ptr) {
  static size_t context = 999999;
  return (uintptr_t)&context;
}

void __envoy_dynamic_module_v1_event_http_filter_destroy(
    __envoy_dynamic_module_v1_type_HttpFilterPtr http_filter_ptr) {}

__envoy_dynamic_module_v1_type_HttpFilterPtr __envoy_dynamic_module_v1_event_http_filter_init(
    __envoy_dynamic_module_v1_type_HttpFilterConfigPtr config_ptr,
    __envoy_dynamic_module_v1_type_HttpFilterConfigSize config_size) {
  static size_t context = 0;
  return (uintptr_t)&context;
}

__envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatus
__envoy_dynamic_module_v1_event_http_filter_instance_request_headers(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestHeadersMapPtr request_headers_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatus
__envoy_dynamic_module_v1_event_http_filter_instance_response_headers(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus
__envoy_dynamic_module_v1_event_http_filter_instance_request_body(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus
__envoy_dynamic_module_v1_event_http_filter_instance_response_body(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

void __envoy_dynamic_module_v1_event_http_filter_instance_destroy(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr) {}

size_t __envoy_dynamic_module_v1_event_program_init() { return 0; }
