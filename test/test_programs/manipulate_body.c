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
  if (end_of_stream) {
    // Append "!" to the buffer.
    __envoy_dynamic_module_v1_http_append_request_body_buffer(buffer, (uintptr_t) "!", 1);

    // Drain "hello ".
    __envoy_dynamic_module_v1_http_drain_request_body_buffer(buffer, 6);

    // Prepend "Envoy" to the buffer.
    __envoy_dynamic_module_v1_http_prepend_request_body_buffer(buffer, (uintptr_t) "Envoy", 5);
  } else {
    // Expect the initial buffer to be hello world.
    size_t size = __envoy_dynamic_module_v1_http_get_request_body_buffer_length(buffer);
    if (size != 11) {
      printf("size must be 11 but got %zu\n", size);
      exit(9999);
    }

    // Replaces all bytes with E
    size_t slices_count =
        __envoy_dynamic_module_v1_http_get_request_body_buffer_slices_count(buffer);
    for (size_t i = 0; i < slices_count; i++) {
      __envoy_dynamic_module_v1_type_DataSlicePtr slice;
      __envoy_dynamic_module_v1_type_DataSliceLength slice_length;
      __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(
          buffer, i, (__envoy_dynamic_module_v1_type_DataSlicePtrResult)&slice,
          (__envoy_dynamic_module_v1_type_DataSliceLengthResult)&slice_length);
      for (size_t j = 0; j < slice_length; j++) {
        ((char*)slice)[j] = 'E';
      }
    }
  }
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus
__envoy_dynamic_module_v1_event_http_filter_instance_response_body(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  if (end_of_stream) {
    // Append "!" to the buffer.
    __envoy_dynamic_module_v1_http_append_response_body_buffer(buffer, (uintptr_t) "!", 1);

    // Drain "hello ".
    __envoy_dynamic_module_v1_http_drain_response_body_buffer(buffer, 6);

    // Prepend "Envoy" to the buffer.
    __envoy_dynamic_module_v1_http_prepend_response_body_buffer(buffer, (uintptr_t) "Envoy", 5);
  } else {
    // Expect the initial buffer to be hello world.
    size_t size = __envoy_dynamic_module_v1_http_get_response_body_buffer_length(buffer);
    if (size != 11) {
      printf("size must be 11 but got %zu\n", size);
      exit(9999);
    }

    // Replaces all bytes with E
    size_t slices_count =
        __envoy_dynamic_module_v1_http_get_response_body_buffer_slices_count(buffer);
    for (size_t i = 0; i < slices_count; i++) {
      __envoy_dynamic_module_v1_type_DataSlicePtr slice;
      __envoy_dynamic_module_v1_type_DataSliceLength slice_length;
      __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(
          buffer, i, (__envoy_dynamic_module_v1_type_DataSlicePtrResult)&slice,
          (__envoy_dynamic_module_v1_type_DataSliceLengthResult)&slice_length);
      for (size_t j = 0; j < slice_length; j++) {
        ((char*)slice)[j] = 'E';
      }
    }
  }
  return 0;
}

__envoy_dynamic_module_v1_type_HttpFilterPtr __envoy_dynamic_module_v1_event_http_filter_init(
    __envoy_dynamic_module_v1_type_HttpFilterConfigPtr config_ptr,
    __envoy_dynamic_module_v1_type_HttpFilterConfigSize config_size) {
  static size_t obj = 0;
  return (uintptr_t)&obj;
}

__envoy_dynamic_module_v1_type_HttpFilterInstancePtr
__envoy_dynamic_module_v1_event_http_filter_instance_init(
    __envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpFilterPtr http_filter_ptr) {
  static size_t obj = 0;
  return (uintptr_t)&obj;
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

size_t __envoy_dynamic_module_v1_event_program_init() { return 0; }
