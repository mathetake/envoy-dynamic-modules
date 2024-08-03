
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "source/extensions/dynamic_modules/abi/abi.h"

static envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter;

envoy_dynamic_module_type_EventHttpRequestBodyStatus
envoy_dynamic_module_event_http_filter_instance_request_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  printf("response: end_of_stream %d: length=%zu\n", end_of_stream != 0,
         envoy_dynamic_module_http_get_request_body_buffer_length(buffer));
  if (!end_of_stream) {
    return 1;
  }
  envoy_dynamic_module_type_HttpRequestBodyBufferPtr entire_buffer =
      envoy_dynamic_module_http_get_request_body_buffer(envoy_filter);
  printf("response: entire_buffer length=%zu\n",
         envoy_dynamic_module_http_get_request_body_buffer_length(entire_buffer));

  // Replaces all buffer bytes with 'A'.
  for (size_t i = 0;
       i < envoy_dynamic_module_http_get_request_body_buffer_slices_count(entire_buffer); i++) {
    char* slice = NULL;
    size_t slice_length = 0;
    envoy_dynamic_module_http_get_request_body_buffer_slice(
        entire_buffer, i, (envoy_dynamic_module_type_DataSlicePtrResult)&slice,
        (envoy_dynamic_module_type_DataSliceLength)&slice_length);

    memset(slice, 'A', slice_length);
  }
  return envoy_dynamic_module_type_EventHttpRequestBodyStatusContinue;
}

envoy_dynamic_module_type_EventHttpResponseBodyStatus
envoy_dynamic_module_event_http_filter_instance_response_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  printf("response: end_of_stream %d: length=%zu\n", end_of_stream != 0,
         envoy_dynamic_module_http_get_response_body_buffer_length(buffer));
  if (!end_of_stream) {
    return 1;
  }
  envoy_dynamic_module_type_HttpResponseBodyBufferPtr entire_buffer =
      envoy_dynamic_module_http_get_response_body_buffer(envoy_filter);
  printf("response: entire_buffer length=%zu\n",
         envoy_dynamic_module_http_get_response_body_buffer_length(entire_buffer));

  // Replaces all buffer bytes with 'B'.
  for (size_t i = 0;
       i < envoy_dynamic_module_http_get_response_body_buffer_slices_count(entire_buffer); i++) {
    char* slice = NULL;
    size_t slice_length = 0;
    envoy_dynamic_module_http_get_response_body_buffer_slice(
        entire_buffer, i, (envoy_dynamic_module_type_DataSlicePtrResult)&slice,
        (envoy_dynamic_module_type_DataSliceLength)&slice_length);

    memset(slice, 'B', slice_length);
  }
  return envoy_dynamic_module_type_EventHttpResponseBodyStatusContinue;
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
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return 0;
}

envoy_dynamic_module_type_HttpFilterPtr envoy_dynamic_module_event_http_filter_init(
    envoy_dynamic_module_type_HttpFilterConfigPtr config_ptr,
    envoy_dynamic_module_type_HttpFilterConfigSize config_size) {
  static size_t obj = 0;
  return (uintptr_t)(&obj);
}

envoy_dynamic_module_type_HttpFilterInstancePtr
envoy_dynamic_module_event_http_filter_instance_init(
    envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr) {
  envoy_filter = envoy_filter_instance_ptr;
  static size_t obj = 0;
  return (uintptr_t)(&obj);
}

void envoy_dynamic_module_event_http_filter_instance_destroy(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr) {}

void envoy_dynamic_module_event_http_filter_destroy(
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr) {}

size_t envoy_dynamic_module_event_program_init() { return 0; }
