#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source/extensions/dynamic_modules/abi/abi.h"

envoy_dynamic_module_type_EventHttpRequestHeadersStatus
envoy_dynamic_module_event_http_filter_instance_request_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestHeadersMapPtr request_headers_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  // Get the value of the header "key" from the headers.
  char* key = "key";
  envoy_dynamic_module_type_InModuleBufferLength key_length = 3;
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length;
  envoy_dynamic_module_http_get_request_header_value(
      request_headers_ptr, (uintptr_t)key, key_length,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);
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
  envoy_dynamic_module_http_get_request_header_value(
      request_headers_ptr, (uintptr_t)key, key_length,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);
  // Check if the header value is NULL.
  if (result_buffer_length != 0 || result_buffer_ptr != 0) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  printf("OK\n");
  return 0;
}

envoy_dynamic_module_type_EventHttpResponseHeadersStatus
envoy_dynamic_module_event_http_filter_instance_response_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  // Get the value of the header "foo" from the headers.
  char* key = "foo";
  size_t key_length = 3;
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  size_t result_buffer_length;
  envoy_dynamic_module_http_get_response_header_value(
      response_headers_map_ptr, (uintptr_t)key, key_length,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);
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
  envoy_dynamic_module_http_get_response_header_value(
      response_headers_map_ptr, (uintptr_t)key, key_length,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);
  // Check if the header value is NULL.
  if (result_buffer_length != 0 || result_buffer_ptr != 0) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  printf("OK\n");
  return 0;
}

envoy_dynamic_module_type_EventHttpRequestBodyStatus
envoy_dynamic_module_event_http_filter_instance_request_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return 0;
}

envoy_dynamic_module_type_HttpFilterPtr envoy_dynamic_module_event_http_filter_init(
    envoy_dynamic_module_type_HttpFilterConfigPtr config_ptr,
    envoy_dynamic_module_type_HttpFilterConfigSize config_size) {
  static size_t obj = 0;
  return (uintptr_t)&obj;
}

envoy_dynamic_module_type_HttpFilterInstancePtr
envoy_dynamic_module_event_http_filter_instance_init(
    envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr) {
  static size_t obj = 0;
  return (uintptr_t)&obj;
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

size_t envoy_dynamic_module_event_program_init() { return 0; }