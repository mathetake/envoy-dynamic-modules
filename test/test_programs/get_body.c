#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abi/abi.h"

envoy_dynamic_module_type_EventHttpRequestBodyStatus
envoy_dynamic_module_event_http_filter_instance_request_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {

  size_t slices_count = envoy_dynamic_module_http_get_request_body_buffer_slices_count(buffer);
  if (slices_count != 2) {
    printf("slice_count must be 2 but got %zu\n", slices_count);
    exit(9999);
  }

  for (size_t i = 0; i < slices_count; i++) {
    envoy_dynamic_module_type_DataSlicePtr result_buffer_ptr;
    envoy_dynamic_module_type_DataSliceLength result_buffer_length;
    envoy_dynamic_module_http_get_request_body_buffer_slice(
        buffer, i, (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
        (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);
    if (i == 0) {
      if (result_buffer_length != 5 ||
          strncmp((char*)result_buffer_ptr, "hello", result_buffer_length) != 0) {
        printf("result[0] does not match: %s expected hello\n",
               strndup((char*)result_buffer_ptr, result_buffer_length));
        exit(9999);
      }
    } else if (i == 1) {
      if (result_buffer_length != 5 ||
          strncmp((char*)result_buffer_ptr, "world", result_buffer_length) != 0) {
        printf("result[1] does not match: %s expected world\n",
               strndup((char*)result_buffer_ptr, result_buffer_length));
        exit(9999);
      }
    }
  }

  // Invalid n-th slice.
  envoy_dynamic_module_type_DataSlicePtr result_buffer_ptr;
  envoy_dynamic_module_type_DataSliceLength result_buffer_length;
  envoy_dynamic_module_http_get_request_body_buffer_slice(
      buffer, 2, (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);
  if (result_buffer_length != 0 || result_buffer_ptr != 0) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }
  return 0;
}

envoy_dynamic_module_type_EventHttpResponseBodyStatus
envoy_dynamic_module_event_http_filter_instance_response_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  size_t slices_count = envoy_dynamic_module_http_get_response_body_buffer_slices_count(buffer);
  if (slices_count != 2) {
    printf("slice_count must be 2 but got %zu\n", slices_count);
    exit(9999);
  }

  for (size_t i = 0; i < slices_count; i++) {
    envoy_dynamic_module_type_DataSlicePtr result_buffer_ptr;
    envoy_dynamic_module_type_DataSliceLength result_buffer_length;
    envoy_dynamic_module_http_get_response_body_buffer_slice(
        buffer, i, (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
        (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);
    if (i == 0) {
      if (result_buffer_length != 2 ||
          strncmp((char*)result_buffer_ptr, "Go", result_buffer_length) != 0) {
        printf("result does not match: %s expected Go\n",
               strndup((char*)result_buffer_ptr, result_buffer_length));
        exit(9999);
      }
    } else if (i == 1) {
      if (result_buffer_length != 4 ||
          strncmp((char*)result_buffer_ptr, "Rust", result_buffer_length) != 0) {
        printf("result does not match: %s expected Rust\n",
               strndup((char*)result_buffer_ptr, result_buffer_length));
        exit(9999);
      }
    }
  }

  // Invalid n-th slice.
  envoy_dynamic_module_type_DataSlicePtr result_buffer_ptr;
  envoy_dynamic_module_type_DataSliceLength result_buffer_length;
  envoy_dynamic_module_http_get_response_body_buffer_slice(
      buffer, 2, (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);
  if (result_buffer_length != 0 || result_buffer_ptr != 0) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }
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

envoy_dynamic_module_type_EventHttpRequestHeadersStatus
envoy_dynamic_module_event_http_filter_instance_request_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestHeadersMapPtr request_headers_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return 0;
}

envoy_dynamic_module_type_EventHttpResponseHeadersStatus
envoy_dynamic_module_event_http_filter_instance_response_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return 0;
}

void envoy_dynamic_module_event_http_filter_instance_destroy(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr) {}

void envoy_dynamic_module_event_http_filter_destroy(
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr) {}

size_t envoy_dynamic_module_event_program_init() { return 0; }