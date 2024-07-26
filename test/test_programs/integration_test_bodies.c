
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "abi.h"

static __envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter;

__envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus
__envoy_dynamic_module_v1_event_http_filter_instance_request_body(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  printf("response: end_of_stream %d: length=%zu\n", end_of_stream != 0,
         __envoy_dynamic_module_v1_http_get_request_body_buffer_length(buffer));
  if (!end_of_stream) {
    return 1;
  }
  __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr entire_buffer =
      __envoy_dynamic_module_v1_http_get_request_body_buffer(envoy_filter);
  printf("response: entire_buffer length=%zu\n",
         __envoy_dynamic_module_v1_http_get_request_body_buffer_length(entire_buffer));

  // Replaces all buffer bytes with 'A'.
  for (size_t i = 0;
       i < __envoy_dynamic_module_v1_http_get_request_body_buffer_slices_count(entire_buffer);
       i++) {
    char* slice = NULL;
    size_t slice_length = 0;
    __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(
        entire_buffer, i, (__envoy_dynamic_module_v1_type_DataSlicePtrResult)&slice,
        (__envoy_dynamic_module_v1_type_DataSliceLength)&slice_length);

    memset(slice, 'A', slice_length);
  }
  return __envoy_dynamic_module_v1_type_EventHttpRequestBodyStatusContinue;
}

__envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus
__envoy_dynamic_module_v1_event_http_filter_instance_response_body(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  printf("response: end_of_stream %d: length=%zu\n", end_of_stream != 0,
         __envoy_dynamic_module_v1_http_get_response_body_buffer_length(buffer));
  if (!end_of_stream) {
    return 1;
  }
  __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr entire_buffer =
      __envoy_dynamic_module_v1_http_get_response_body_buffer(envoy_filter);
  printf("response: entire_buffer length=%zu\n",
         __envoy_dynamic_module_v1_http_get_response_body_buffer_length(entire_buffer));

  // Replaces all buffer bytes with 'B'.
  for (size_t i = 0;
       i < __envoy_dynamic_module_v1_http_get_response_body_buffer_slices_count(entire_buffer);
       i++) {
    char* slice = NULL;
    size_t slice_length = 0;
    __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(
        entire_buffer, i, (__envoy_dynamic_module_v1_type_DataSlicePtrResult)&slice,
        (__envoy_dynamic_module_v1_type_DataSliceLength)&slice_length);

    memset(slice, 'B', slice_length);
  }
  return __envoy_dynamic_module_v1_type_EventHttpResponseBodyStatusContinue;
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
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_HttpFilterPtr __envoy_dynamic_module_v1_event_http_filter_init(
    __envoy_dynamic_module_v1_type_HttpFilterConfigPtr config_ptr,
    __envoy_dynamic_module_v1_type_HttpFilterConfigSize config_size) {
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
