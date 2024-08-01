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
  if (end_of_stream) {
    // Append "!" to the buffer.
    envoy_dynamic_module_http_append_request_body_buffer(buffer, (uintptr_t) "!", 1);

    // Drain "hello ".
    envoy_dynamic_module_http_drain_request_body_buffer(buffer, 6);

    // Prepend "Envoy" to the buffer.
    envoy_dynamic_module_http_prepend_request_body_buffer(buffer, (uintptr_t) "Envoy", 5);
  } else {
    // Expect the initial buffer to be hello world.
    size_t size = envoy_dynamic_module_http_get_request_body_buffer_length(buffer);
    if (size != 11) {
      printf("size must be 11 but got %zu\n", size);
      exit(9999);
    }

    // Replaces all bytes with E
    size_t slices_count = envoy_dynamic_module_http_get_request_body_buffer_slices_count(buffer);
    for (size_t i = 0; i < slices_count; i++) {
      envoy_dynamic_module_type_DataSlicePtr slice;
      envoy_dynamic_module_type_DataSliceLength slice_length;
      envoy_dynamic_module_http_get_request_body_buffer_slice(
          buffer, i, (envoy_dynamic_module_type_DataSlicePtrResult)&slice,
          (envoy_dynamic_module_type_DataSliceLengthResult)&slice_length);
      for (size_t j = 0; j < slice_length; j++) {
        ((char*)slice)[j] = 'E';
      }
    }
  }
  return 0;
}

envoy_dynamic_module_type_EventHttpResponseBodyStatus
envoy_dynamic_module_event_http_filter_instance_response_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  if (end_of_stream) {
    // Append "!" to the buffer.
    envoy_dynamic_module_http_append_response_body_buffer(buffer, (uintptr_t) "!", 1);

    // Drain "hello ".
    envoy_dynamic_module_http_drain_response_body_buffer(buffer, 6);

    // Prepend "Envoy" to the buffer.
    envoy_dynamic_module_http_prepend_response_body_buffer(buffer, (uintptr_t) "Envoy", 5);
  } else {
    // Expect the initial buffer to be hello world.
    size_t size = envoy_dynamic_module_http_get_response_body_buffer_length(buffer);
    if (size != 11) {
      printf("size must be 11 but got %zu\n", size);
      exit(9999);
    }

    // Replaces all bytes with E
    size_t slices_count = envoy_dynamic_module_http_get_response_body_buffer_slices_count(buffer);
    for (size_t i = 0; i < slices_count; i++) {
      envoy_dynamic_module_type_DataSlicePtr slice;
      envoy_dynamic_module_type_DataSliceLength slice_length;
      envoy_dynamic_module_http_get_response_body_buffer_slice(
          buffer, i, (envoy_dynamic_module_type_DataSlicePtrResult)&slice,
          (envoy_dynamic_module_type_DataSliceLengthResult)&slice_length);
      for (size_t j = 0; j < slice_length; j++) {
        ((char*)slice)[j] = 'E';
      }
    }
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
