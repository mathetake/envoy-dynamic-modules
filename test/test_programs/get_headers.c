#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abi.h"

__envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatus
__envoy_dynamic_module_v1_event_http_filter_instance_request_headers(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestHeadersMapPtr request_headers_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  // Get the value of the header "key" from the headers.
  char* key = "key";
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = 3;
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length;
  __envoy_dynamic_module_v1_http_get_request_header_value(
      request_headers_ptr, (uintptr_t)key, key_length, &result_buffer_ptr, &result_buffer_length);
  // Check if the header value is "value".
  char* value = "value";
  if (result_buffer_length != 5 ||
      strncmp((char*)result_buffer_ptr, value, result_buffer_length) != 0) {
    printf("result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  // Get the non-existent header "non-existent-key" from the headers.
  key = "non-existent-key";
  key_length = 15;
  __envoy_dynamic_module_v1_http_get_request_header_value(
      request_headers_ptr, (uintptr_t)key, key_length, &result_buffer_ptr, &result_buffer_length);
  // Check if the header value is NULL.
  if (result_buffer_length != 0 || result_buffer_ptr != 0) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  printf("OK\n");
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatus
__envoy_dynamic_module_v1_event_http_filter_instance_response_headers(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  // Get the value of the header "foo" from the headers.
  char* key = "foo";
  size_t key_length = 3;
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  size_t result_buffer_length;
  __envoy_dynamic_module_v1_http_get_response_header_value(response_headers_map_ptr, (uintptr_t)key,
                                                           key_length, &result_buffer_ptr,
                                                           &result_buffer_length);
  // Check if the header value is "value".
  char* value = "bar";
  if (result_buffer_length != 3 ||
      strncmp((char*)result_buffer_ptr, value, result_buffer_length) != 0) {
    printf("result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  // Get the non-existent header "non-existent-key" from the headers.
  key = "non-existent-key";
  key_length = 15;
  __envoy_dynamic_module_v1_http_get_response_header_value(response_headers_map_ptr, (uintptr_t)key,
                                                           key_length, &result_buffer_ptr,
                                                           &result_buffer_length);
  // Check if the header value is NULL.
  if (result_buffer_length != 0 || result_buffer_ptr != 0) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  printf("OK\n");
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus
__envoy_dynamic_module_v1_event_http_filter_instance_request_body(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_HttpFilterPtr __envoy_dynamic_module_v1_event_http_filter_init(
    __envoy_dynamic_module_v1_type_HttpFilterConfigPtr config_ptr,
    __envoy_dynamic_module_v1_type_HttpFilterConfigSize config_size) {
  static size_t context = 0;
  return (uintptr_t)&context;
}

__envoy_dynamic_module_v1_type_HttpFilterInstancePtr
__envoy_dynamic_module_v1_event_http_filter_instance_init(
    __envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpFilterPtr http_filter_ptr) {
  static size_t context = 0;
  return (uintptr_t)&context;
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

void __envoy_dynamic_module_v1_event_http_filter_destroy(
    __envoy_dynamic_module_v1_type_HttpFilterPtr http_filter_ptr) {}
