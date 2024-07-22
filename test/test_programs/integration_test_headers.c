
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "abi.h"

size_t should_wait = 0;

struct httpContext {
  __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr;
};

__envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatus
__envoy_dynamic_module_v1_event_http_request_headers(
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestHeadersMapPtr request_headers_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

void* sleep_thread(__envoy_dynamic_module_v1_type_EnvoyFilterPtr arg) {
  printf("sleeping for 1 second\n");
  sleep(1);
  printf("continuing\n");
  __envoy_dynamic_module_v1_http_continue_response(arg);
  return NULL;
}

__envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatus
__envoy_dynamic_module_v1_event_http_response_headers(
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  struct httpContext* http_context = (struct httpContext*)http_context_ptr;
  if (should_wait) {
    // Spwans a new thread and waits for 1 second.
    pthread_t thread;
    pthread_create(&thread, NULL, sleep_thread, (void*)http_context->envoy_filter_ptr);
    printf("returning stop iteration\n");
    return __envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatusStopAllIterationAndBuffer;
  }

  // Get the status code and assert it is 404.
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  size_t result_buffer_length;
  __envoy_dynamic_module_v1_http_get_response_header_value(
      response_headers_map_ptr, ":status", 7, &result_buffer_ptr, &result_buffer_length);

  if (result_buffer_length != 3 || strncmp(result_buffer_ptr, "404", result_buffer_length) != 0) {
    printf("status code is not 404: %s\n", strndup((char*)result_buffer_ptr, result_buffer_length));
    exit(9999);
  }
  // Change the status code.
  __envoy_dynamic_module_v1_http_set_response_header(response_headers_map_ptr, ":status", 7, "200",
                                                     3);

  // Add a new header.
  __envoy_dynamic_module_v1_http_set_response_header(response_headers_map_ptr, "new_key", 7,
                                                     "value", 5);
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus
__envoy_dynamic_module_v1_event_http_request_body(
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_ModuleContextPtr __envoy_dynamic_module_v1_event_module_init(
    __envoy_dynamic_module_v1_type_ModuleConfigPtr config_ptr,
    __envoy_dynamic_module_v1_type_ModuleConfigSize config_size) {
  // Check if the config string equals "should_wait".
  if (config_size == 11 && strncmp(config_ptr, "should_wait", 11) == 0) {
    should_wait = 1;
  }

  static size_t context = 0;
  return (uintptr_t)(&context);
}

__envoy_dynamic_module_v1_type_HttpContextPtr __envoy_dynamic_module_v1_event_http_context_init(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_ModuleContextPtr module_ctx_ptr) {
  struct httpContext* context = malloc(sizeof(struct httpContext));
  context->envoy_filter_ptr = envoy_filter_ptr;
  return (uintptr_t)context;
}

__envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus
__envoy_dynamic_module_v1_event_http_response_body(
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

void __envoy_dynamic_module_v1_event_http_destroy(
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr) {}
