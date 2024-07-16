#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "x/abi.h"

int already_called = 0;

size_t __envoy_dynamic_module_v1_event_module_init(
    __envoy_dynamic_module_v1_type_ModuleConfigPtr config_ptr,
    __envoy_dynamic_module_v1_type_ModuleConfigSize config_size) {
  printf("\t\tinit called with config: %s and already_called_ptr: %p, already_called: %d\n",
         config_ptr, &already_called, already_called);
  if (already_called == 1) {
    // This function should only be called once.
    return 1;
  }
  already_called = 1;

  printf("init called with config: %s and already_called_ptr: %p, already_called: %d\n", config_ptr,
         &already_called, already_called);

  if (config_size != 6) {
    return 1;
  }

  // Checks if the config is equals to "config"
  if (strcmp(config_ptr, "config") != 0) {
    return 1;
  }
  // Replaces the config with "11111" via for loop.
  for (int i = 0; i < 6; i++) {
    // Cast the pointer to non-const to modify the value for testing.
    ((char*)config_ptr)[i] = '1';
  }
  return 0;
}

__envoy_dynamic_module_v1_type_HttpContextPtr __envoy_dynamic_module_v1_event_http_context_init() {
  return NULL;
}

__envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatus
__envoy_dynamic_module_v1_event_http_request_headers(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_RequestHeadersMapPtr request_headers_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus
__envoy_dynamic_module_v1_event_http_request_body(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_RequestBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatus
__envoy_dynamic_module_v1_event_http_response_headers(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_ResponseHeaderMapPtr response_headers_map_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus
__envoy_dynamic_module_v1_event_http_response_body(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_ResponseBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

void __envoy_dynamic_module_v1_http_on_destroy(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr) {}