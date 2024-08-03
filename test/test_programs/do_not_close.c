#include "source/extensions/dynamic_modules/abi/abi.h"

size_t envoy_dynamic_module_event_program_init() { return 0; }

envoy_dynamic_module_type_HttpFilterPtr envoy_dynamic_module_event_http_filter_init(
    envoy_dynamic_module_type_HttpFilterConfigPtr config_ptr,
    envoy_dynamic_module_type_HttpFilterConfigSize config_size) {
  static size_t obj = 999999;
  return (uintptr_t)&obj;
}

envoy_dynamic_module_type_HttpFilterInstancePtr
envoy_dynamic_module_event_http_filter_instance_init(
    envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr) {
  return 0;
}

envoy_dynamic_module_type_EventHttpRequestHeadersStatus
envoy_dynamic_module_event_http_filter_instance_request_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestHeadersMapPtr request_headers_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return 0;
}

envoy_dynamic_module_type_EventHttpRequestBodyStatus
envoy_dynamic_module_event_http_filter_instance_request_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return 0;
}

envoy_dynamic_module_type_EventHttpResponseHeadersStatus
envoy_dynamic_module_event_http_filter_instance_response_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return 0;
}

envoy_dynamic_module_type_EventHttpResponseBodyStatus
envoy_dynamic_module_event_http_filter_instance_response_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return 0;
}

void envoy_dynamic_module_event_http_filter_instance_destroy(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr) {}

void envoy_dynamic_module_event_http_filter_destroy(
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr) {}
