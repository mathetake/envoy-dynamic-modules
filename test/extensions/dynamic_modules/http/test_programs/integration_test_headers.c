
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "source/extensions/dynamic_modules/abi/abi.h"

size_t should_wait = 0;

struct httpContext {
  envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr;
};

envoy_dynamic_module_type_EventHttpRequestHeadersStatus
envoy_dynamic_module_on_http_filter_instance_request_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestHeadersMapPtr request_headers_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return 0;
}

void* sleep_thread(envoy_dynamic_module_type_EnvoyFilterInstancePtr arg) {
  printf("sleeping for 1 second\n");
  sleep(1);
  printf("continuing\n");
  envoy_dynamic_module_http_continue_response(arg);
  return NULL;
}

envoy_dynamic_module_type_EventHttpResponseHeadersStatus
envoy_dynamic_module_on_http_filter_instance_response_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  struct httpContext* http_obj = (struct httpContext*)http_filter_instance_ptr;
  if (should_wait) {
    // Spwans a new thread and waits for 1 second.
    pthread_t thread;
    pthread_create(&thread, NULL, (void*)sleep_thread, (void*)http_obj->envoy_filter_instance_ptr);
    printf("returning stop iteration\n");
    return envoy_dynamic_module_type_EventHttpRequestHeadersStatusStopAllIterationAndBuffer;
  }

  // Get the status code and assert it is 404.
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  size_t result_buffer_length;
  envoy_dynamic_module_http_get_response_header_value(
      response_headers_map_ptr, (uintptr_t) ":status", 7,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);

  if (result_buffer_length != 3 ||
      strncmp((char*)result_buffer_ptr, "404", result_buffer_length) != 0) {
    printf("status code is not 404: %s\n", strndup((char*)result_buffer_ptr, result_buffer_length));
    exit(9999);
  }
  // Change the status code.
  envoy_dynamic_module_http_set_response_header(response_headers_map_ptr, (uintptr_t) ":status", 7,
                                                (uintptr_t) "200", 3);

  // Add a new header.
  envoy_dynamic_module_http_set_response_header(response_headers_map_ptr, (uintptr_t) "new_key", 7,
                                                (uintptr_t) "value", 5);
  return 0;
}

envoy_dynamic_module_type_EventHttpRequestBodyStatus
envoy_dynamic_module_on_http_filter_instance_request_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return 0;
}

envoy_dynamic_module_type_HttpFilterPtr envoy_dynamic_module_on_http_filter_init(
    envoy_dynamic_module_type_HttpFilterConfigPtr config_ptr,
    envoy_dynamic_module_type_HttpFilterConfigSize config_size) {
  // Check if the config string equals "should_wait".
  if (config_size == 11 && strncmp((char*)config_ptr, "should_wait", 11) == 0) {
    should_wait = 1;
  }

  static size_t obj = 0;
  return (uintptr_t)(&obj);
}

envoy_dynamic_module_type_HttpFilterInstancePtr envoy_dynamic_module_on_http_filter_instance_init(
    envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr) {
  struct httpContext* obj = malloc(sizeof(struct httpContext));
  obj->envoy_filter_instance_ptr = envoy_filter_instance_ptr;
  return (uintptr_t)obj;
}

envoy_dynamic_module_type_EventHttpResponseBodyStatus
envoy_dynamic_module_on_http_filter_instance_response_body(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseBodyBufferPtr buffer,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  return 0;
}

void envoy_dynamic_module_on_http_filter_instance_destroy(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr) {}

void envoy_dynamic_module_on_http_filter_destroy(
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr) {}

size_t envoy_dynamic_module_on_program_init() { return 0; }
