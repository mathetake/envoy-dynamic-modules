#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "x/abi.h"

__envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatus
__envoy_dynamic_module_v1_event_http_request_headers(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_RequestHeadersMapPtr request_headers_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  // Get the value of the header "key" from the headers.
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key = "key";
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = 3;
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length;
  __envoy_dynamic_module_v1_http_get_request_header_value(
      request_headers_ptr, key, key_length, &result_buffer_ptr, &result_buffer_length);
  // Check if the header value is "value".
  __envoy_dynamic_module_v1_type_InModuleBufferPtr value = "value";
  if (result_buffer_length != 5 || strncmp(result_buffer_ptr, value, result_buffer_length) != 0) {
    printf("result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  // Get the non-existent header "non-existent-key" from the headers.
  key = "non-existent-key";
  key_length = 15;
  __envoy_dynamic_module_v1_http_get_request_header_value(
      request_headers_ptr, key, key_length, &result_buffer_ptr, &result_buffer_length);
  // Check if the header value is NULL.
  if (result_buffer_length != 0 || result_buffer_ptr != NULL) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  printf("OK\n");
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatus
__envoy_dynamic_module_v1_event_http_response_headers(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_ResponseHeaderMapPtr response_headers_map_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  // Get the value of the header "foo" from the headers.
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key = "foo";
  size_t key_length = 3;
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  size_t result_buffer_length;
  __envoy_dynamic_module_v1_http_get_response_header_value(
      response_headers_map_ptr, key, key_length, &result_buffer_ptr, &result_buffer_length);
  // Check if the header value is "value".
  __envoy_dynamic_module_v1_type_InModuleBufferPtr value = "bar";
  if (result_buffer_length != 3 || strncmp(result_buffer_ptr, value, result_buffer_length) != 0) {
    printf("result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  // Get the non-existent header "non-existent-key" from the headers.
  key = "non-existent-key";
  key_length = 15;
  __envoy_dynamic_module_v1_http_get_response_header_value(
      response_headers_map_ptr, key, key_length, &result_buffer_ptr, &result_buffer_length);
  // Check if the header value is NULL.
  if (result_buffer_length != 0 || result_buffer_ptr != NULL) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  printf("OK\n");
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus
__envoy_dynamic_module_v1_event_http_request_body(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_RequestBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

size_t __envoy_dynamic_module_v1_event_module_init(
    __envoy_dynamic_module_v1_type_ModuleConfigPtr config_ptr,
    __envoy_dynamic_module_v1_type_ModuleConfigSize config_size) {
  return 0;
}

size_t context = 0;

void* __envoy_dynamic_module_v1_event_http_context_init() {
  context = 999999;
  return &context;
}

__envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus
__envoy_dynamic_module_v1_event_http_response_body(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_ResponseBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

void __envoy_dynamic_module_v1_http_on_destroy(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr) {}
