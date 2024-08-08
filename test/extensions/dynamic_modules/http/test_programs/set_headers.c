#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source/extensions/dynamic_modules/abi/abi.h"

envoy_dynamic_module_type_EventHttpRequestHeadersStatus
envoy_dynamic_module_on_http_filter_instance_request_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpRequestHeadersMapPtr request_headers_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  {
    void* key = "new_key";
    size_t key_length = 7;
    void* value = "value";
    size_t value_length = 5;
    envoy_dynamic_module_http_set_request_header(request_headers_ptr, (uintptr_t)key, key_length,
                                                 (uintptr_t)value, value_length);
  }
  {
    void* key = "existing_key";
    size_t key_length = 12;
    void* value = "new_value";
    size_t value_length = 9;
    envoy_dynamic_module_http_set_request_header(request_headers_ptr, (uintptr_t)key, key_length,
                                                 (uintptr_t)value, value_length);
  }
  {
    void* key = "to_delete";
    size_t key_length = 9;
    void* value = NULL;
    size_t value_length = 0;
    envoy_dynamic_module_http_set_request_header(request_headers_ptr, (uintptr_t)key, key_length,
                                                 (uintptr_t)value, value_length);
  }
  {
    void* key = "existing_key_with_multiple_values";
    size_t key_length = 32;
    void* value = "unique_value";
    size_t value_length = 12;
    envoy_dynamic_module_http_set_request_header(request_headers_ptr, (uintptr_t)key, key_length,
                                                 (uintptr_t)value, value_length);
  }
  return 0;
}

envoy_dynamic_module_type_EventHttpResponseHeadersStatus
envoy_dynamic_module_on_http_filter_instance_response_headers(
    envoy_dynamic_module_type_HttpFilterInstancePtr http_filter_instance_ptr,
    envoy_dynamic_module_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    envoy_dynamic_module_type_EndOfStream end_of_stream) {
  {
    void* key = "new_key";
    size_t key_length = 7;
    void* value = "value";
    size_t value_length = 5;
    envoy_dynamic_module_http_set_response_header(response_headers_map_ptr, (uintptr_t)key,
                                                  key_length, (uintptr_t)value, value_length);
  }
  {
    void* key = "existing_key";
    size_t key_length = 12;
    void* value = "new_value";
    size_t value_length = 9;
    envoy_dynamic_module_http_set_response_header(response_headers_map_ptr, (uintptr_t)key,
                                                  key_length, (uintptr_t)value, value_length);
  }
  {
    void* key = "to_delete";
    size_t key_length = 9;
    void* value = NULL;
    size_t value_length = 0;
    envoy_dynamic_module_http_set_response_header(response_headers_map_ptr, (uintptr_t)key,
                                                  key_length, (uintptr_t)value, value_length);
  }
  {
    void* key = "existing_key_with_multiple_values";
    size_t key_length = 33;
    void* value = "unique_value";
    size_t value_length = 12;
    envoy_dynamic_module_http_set_response_header(response_headers_map_ptr, (uintptr_t)key,
                                                  key_length, (uintptr_t)value, value_length);
  }
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
  static size_t obj = 0;
  return (uintptr_t)&obj;
}

envoy_dynamic_module_type_HttpFilterInstancePtr envoy_dynamic_module_on_http_filter_instance_init(
    envoy_dynamic_module_type_EnvoyFilterInstancePtr envoy_filter_instance_ptr,
    envoy_dynamic_module_type_HttpFilterPtr http_filter_ptr) {
  static size_t obj = 0;
  return (uintptr_t)&obj;
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
