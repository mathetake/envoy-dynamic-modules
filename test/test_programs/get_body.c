#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abi.h"

__envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus
__envoy_dynamic_module_v1_event_http_filter_instance_request_body(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {

  size_t slices_count = __envoy_dynamic_module_v1_http_get_request_body_buffer_slices_count(buffer);
  if (slices_count != 2) {
    printf("slice_count must be 2 but got %zu\n", slices_count);
    exit(9999);
  }

  for (size_t i = 0; i < slices_count; i++) {
    __envoy_dynamic_module_v1_type_DataSlicePtr result_buffer_ptr;
    __envoy_dynamic_module_v1_type_DataSliceLength result_buffer_length;
    __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(buffer, i, &result_buffer_ptr,
                                                                 &result_buffer_length);
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
  __envoy_dynamic_module_v1_type_DataSlicePtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_DataSliceLength result_buffer_length;
  __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(buffer, 2, &result_buffer_ptr,
                                                               &result_buffer_length);
  if (result_buffer_length != 0 || result_buffer_ptr != 0) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus
__envoy_dynamic_module_v1_event_http_filter_instance_response_body(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  size_t slices_count =
      __envoy_dynamic_module_v1_http_get_response_body_buffer_slices_count(buffer);
  if (slices_count != 2) {
    printf("slice_count must be 2 but got %zu\n", slices_count);
    exit(9999);
  }

  for (size_t i = 0; i < slices_count; i++) {
    __envoy_dynamic_module_v1_type_DataSlicePtr result_buffer_ptr;
    __envoy_dynamic_module_v1_type_DataSliceLength result_buffer_length;
    __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(buffer, i, &result_buffer_ptr,
                                                                  &result_buffer_length);
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
  __envoy_dynamic_module_v1_type_DataSlicePtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_DataSliceLength result_buffer_length;
  __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(buffer, 2, &result_buffer_ptr,
                                                                &result_buffer_length);
  if (result_buffer_length != 0 || result_buffer_ptr != 0) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }
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
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

void __envoy_dynamic_module_v1_event_http_filter_instance_destroy(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr) {}

void __envoy_dynamic_module_v1_event_http_filter_destroy(
    __envoy_dynamic_module_v1_type_HttpFilterPtr http_filter_ptr) {}