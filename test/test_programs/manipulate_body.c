#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "x/abi.h"

__envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus
__envoy_dynamic_module_v1_event_http_request_body(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_RequestBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  if (end_of_stream) {
    // Append "!" to the buffer.
    __envoy_dynamic_module_v1_http_get_request_body_append(buffer, "!", 1);

    // Drain "hello ".
    __envoy_dynamic_module_v1_http_get_request_body_buffer_drain(buffer, 6);

    // Prepend "Envoy" to the buffer.
    __envoy_dynamic_module_v1_http_get_request_body_prepend(buffer, "Envoy", 5);
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
      __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(buffer, i, &slice,
                                                                   &slice_length);
      for (size_t j = 0; j < slice_length; j++) {
        ((char*)slice)[j] = 'E';
      }
    }
  }
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus
__envoy_dynamic_module_v1_event_http_response_body(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_ResponseBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  if (end_of_stream) {
    // Append "!" to the buffer.
    __envoy_dynamic_module_v1_http_get_response_body_append(buffer, "!", 1);

    // Drain "hello ".
    __envoy_dynamic_module_v1_http_get_response_body_buffer_drain(buffer, 6);

    // Prepend "Envoy" to the buffer.
    __envoy_dynamic_module_v1_http_get_response_body_prepend(buffer, "Envoy", 5);
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
      __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(buffer, i, &slice,
                                                                    &slice_length);
      for (size_t j = 0; j < slice_length; j++) {
        ((char*)slice)[j] = 'E';
      }
    }
  }
  return 0;
}

size_t __envoy_dynamic_module_v1_event_module_init(
    __envoy_dynamic_module_v1_type_ModuleConfigPtr config_ptr,
    __envoy_dynamic_module_v1_type_ModuleConfigSize config_size) {
  return 0;
}

size_t context = 0;

__envoy_dynamic_module_v1_type_HttpContextPtr __envoy_dynamic_module_v1_event_http_context_init() {
  context = 999999;
  return &context;
}

__envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatus
__envoy_dynamic_module_v1_event_http_request_headers(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_RequestHeadersMapPtr request_headers_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatus
__envoy_dynamic_module_v1_event_http_response_headers(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_ResponseHeaderMapPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

void __envoy_dynamic_module_v1_http_on_destroy(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr) {}
