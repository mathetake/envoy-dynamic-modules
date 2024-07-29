
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "abi/abi.h"

int on_response = 0;
__envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter;

__envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatus
__envoy_dynamic_module_v1_event_http_filter_instance_request_headers(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestHeadersMapPtr request_headers_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  if (on_response != 0) {
    return __envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatusContinue;
  }

  __envoy_dynamic_module_v1_type_InModuleHeader foo_header;
  foo_header.header_key = (uintptr_t) "foo";
  foo_header.header_key_length = 3;
  foo_header.header_value = (uintptr_t) "bar";
  foo_header.header_value_length = 3;

  printf("sending response from request body\n");

  __envoy_dynamic_module_v1_http_send_response(
      envoy_filter, 500, (__envoy_dynamic_module_v1_type_InModuleHeadersPtr)&foo_header, 1, 0, 0);
  return __envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatusContinue;
}

__envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatus
__envoy_dynamic_module_v1_event_http_filter_instance_response_headers(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  if (on_response != 1) {
    return __envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatusContinue;
  }

  __envoy_dynamic_module_v1_type_InModuleHeader cat_header;
  cat_header.header_key = (uintptr_t) "cat";
  cat_header.header_key_length = 3;
  cat_header.header_value = (uintptr_t) "dog";
  cat_header.header_value_length = 3;

  __envoy_dynamic_module_v1_http_send_response(
      envoy_filter, 504, (__envoy_dynamic_module_v1_type_InModuleHeadersPtr)&cat_header, 1, 0, 0);
  return __envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatusContinue;
}

__envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus
__envoy_dynamic_module_v1_event_http_filter_instance_request_body(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return __envoy_dynamic_module_v1_type_EventHttpResponseBodyStatusContinue;
}

__envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus
__envoy_dynamic_module_v1_event_http_filter_instance_response_body(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return __envoy_dynamic_module_v1_type_EventHttpResponseBodyStatusContinue;
}

__envoy_dynamic_module_v1_type_HttpFilterPtr __envoy_dynamic_module_v1_event_http_filter_init(
    __envoy_dynamic_module_v1_type_HttpFilterConfigPtr config_ptr,
    __envoy_dynamic_module_v1_type_HttpFilterConfigSize config_size) {
  // Check if the config string equals "should_wait".
  if (config_size == 11 && strncmp((char*)config_ptr, "on_response", 11) == 0) {
    on_response = 1;
  }

  static size_t obj = 0;
  return (uintptr_t)(&obj);
}

__envoy_dynamic_module_v1_type_HttpFilterInstancePtr
__envoy_dynamic_module_v1_event_http_filter_instance_init(
    __envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpFilterPtr http_filter_ptr) {
  envoy_filter = envoy_filter_instance_ptr;
  static size_t obj = 0;
  return (uintptr_t)(&obj);
}

void __envoy_dynamic_module_v1_event_http_filter_instance_destroy(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr) {}

void __envoy_dynamic_module_v1_event_http_filter_destroy(
    __envoy_dynamic_module_v1_type_HttpFilterPtr http_filter_ptr) {}

size_t __envoy_dynamic_module_v1_event_program_init() { return 0; }
