
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "abi/abi.h"

size_t should_wait = 0;

struct httpContext {
  __envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr;
};

__envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatus
__envoy_dynamic_module_v1_event_http_filter_instance_request_headers(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestHeadersMapPtr request_headers_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

void* sleep_thread(__envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr arg) {
  printf("sleeping for 1 second\n");
  sleep(1);
  printf("continuing\n");
  __envoy_dynamic_module_v1_http_continue_response(arg);
  return NULL;
}

__envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatus
__envoy_dynamic_module_v1_event_http_filter_instance_response_headers(
    __envoy_dynamic_module_v1_type_HttpFilterInstancePtr http_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  struct httpContext* http_obj = (struct httpContext*)http_filter_instance_ptr;
  if (should_wait) {
    // Spwans a new thread and waits for 1 second.
    pthread_t thread;
    pthread_create(&thread, NULL, (void*)sleep_thread, (void*)http_obj->envoy_filter_instance_ptr);
    printf("returning stop iteration\n");
    return __envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatusStopAllIterationAndBuffer;
  }

  // Get the status code and assert it is 404.
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  size_t result_buffer_length;
  __envoy_dynamic_module_v1_http_get_response_header_value(
      response_headers_map_ptr, (uintptr_t) ":status", 7,
      (__envoy_dynamic_module_v1_type_DataSlicePtrResult)&result_buffer_ptr,
      (__envoy_dynamic_module_v1_type_DataSliceLengthResult)&result_buffer_length);

  if (result_buffer_length != 3 ||
      strncmp((char*)result_buffer_ptr, "404", result_buffer_length) != 0) {
    printf("status code is not 404: %s\n", strndup((char*)result_buffer_ptr, result_buffer_length));
    exit(9999);
  }
  // Change the status code.
  __envoy_dynamic_module_v1_http_set_response_header(
      response_headers_map_ptr, (uintptr_t) ":status", 7, (uintptr_t) "200", 3);

  // Add a new header.
  __envoy_dynamic_module_v1_http_set_response_header(
      response_headers_map_ptr, (uintptr_t) "new_key", 7, (uintptr_t) "value", 5);
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
  // Check if the config string equals "should_wait".
  if (config_size == 11 && strncmp((char*)config_ptr, "should_wait", 11) == 0) {
    should_wait = 1;
  }

  static size_t obj = 0;
  return (uintptr_t)(&obj);
}

__envoy_dynamic_module_v1_type_HttpFilterInstancePtr
__envoy_dynamic_module_v1_event_http_filter_instance_init(
    __envoy_dynamic_module_v1_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    __envoy_dynamic_module_v1_type_HttpFilterPtr http_filter_ptr) {
  struct httpContext* obj = malloc(sizeof(struct httpContext));
  obj->envoy_filter_instance_ptr = envoy_filter_instance_ptr;
  return (uintptr_t)obj;
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

size_t __envoy_dynamic_module_v1_event_program_init() { return 0; }
