
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "abi/abi.h"

int on_response = 0;
envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter;

envoy_dynamic_module_type_EventHttpRequestHeadersStatus
envoy_dynamic_module_event_http_filter_instance_request_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestHeadersMapPtr request_headers_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  if (on_response != 0) {
    return envoy_dynamic_module_type_EventHttpRequestHeadersStatusContinue;
  }

  envoy_dynamic_module_type_InModuleHeader foo_header;
  foo_header.header_key = (uintptr_t) "foo";
  foo_header.header_key_length = 3;
  foo_header.header_value = (uintptr_t) "bar";
  foo_header.header_value_length = 3;

  printf("sending response from request body\n");

  envoy_dynamic_module_http_send_response(
      envoy_filter, 500, (envoy_dynamic_module_type_InModuleHeadersPtr)&foo_header, 1, 0, 0);
  return envoy_dynamic_module_type_EventHttpRequestHeadersStatusContinue;
}

envoy_dynamic_module_type_EventHttpResponseHeadersStatus
envoy_dynamic_module_event_http_filter_instance_response_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  if (on_response != 1) {
    return envoy_dynamic_module_type_EventHttpResponseHeadersStatusContinue;
  }

  envoy_dynamic_module_type_InModuleHeader cat_header;
  cat_header.header_key = (uintptr_t) "cat";
  cat_header.header_key_length = 3;
  cat_header.header_value = (uintptr_t) "dog";
  cat_header.header_value_length = 3;

  envoy_dynamic_module_http_send_response(
      envoy_filter, 504, (envoy_dynamic_module_type_InModuleHeadersPtr)&cat_header, 1, 0, 0);
  return envoy_dynamic_module_type_EventHttpResponseHeadersStatusContinue;
}

envoy_dynamic_module_type_EventHttpRequestBodyStatus
envoy_dynamic_module_event_http_filter_instance_request_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return envoy_dynamic_module_type_EventHttpResponseBodyStatusContinue;
}

envoy_dynamic_module_type_EventHttpResponseBodyStatus
envoy_dynamic_module_event_http_filter_instance_response_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return envoy_dynamic_module_type_EventHttpResponseBodyStatusContinue;
}

envoy_dynamic_module_type_HttpFilterPtr envoy_dynamic_module_event_http_filter_init(
    envoy_dynamic_module_type_HttpFilterConfigPtr config_ptr,
    envoy_dynamic_module_type_HttpFilterConfigSize config_size) {
  // Check if the config string equals "should_wait".
  if (config_size == 11 && strncmp((char*)config_ptr, "on_response", 11) == 0) {
    on_response = 1;
  }

  static size_t obj = 0;
  return (uintptr_t)(&obj);
}

envoy_dynamic_module_type_HttpFilterInstancePtr
envoy_dynamic_module_event_http_filter_instance_init(
    envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr) {
  envoy_filter = envoy_filter_instance_ptr;
  static size_t obj = 0;
  return (uintptr_t)(&obj);
}

void envoy_dynamic_module_event_http_filter_instance_destroy(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr) {}

void envoy_dynamic_module_event_http_filter_destroy(
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr) {}

size_t envoy_dynamic_module_event_program_init() { return 0; }
