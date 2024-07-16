#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "x/abi.h"

__envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatus
__envoy_dynamic_module_v1_event_http_request_headers(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestHeadersMapPtr request_headers_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  {
    void* key = "new_key";
    size_t key_length = 7;
    void* value = "value";
    size_t value_length = 5;
    __envoy_dynamic_module_v1_http_set_request_header(request_headers_ptr, key, key_length, value,
                                                      value_length);
  }
  {
    void* key = "existing_key";
    size_t key_length = 12;
    void* value = "new_value";
    size_t value_length = 9;
    __envoy_dynamic_module_v1_http_set_request_header(request_headers_ptr, key, key_length, value,
                                                      value_length);
  }
  {
    void* key = "to_delete";
    size_t key_length = 9;
    void* value = NULL;
    size_t value_length = 0;
    __envoy_dynamic_module_v1_http_set_request_header(request_headers_ptr, key, key_length, value,
                                                      value_length);
  }
  {
    void* key = "existing_key_with_multiple_values";
    size_t key_length = 32;
    void* value = "unique_value";
    size_t value_length = 12;
    __envoy_dynamic_module_v1_http_set_request_header(request_headers_ptr, key, key_length, value,
                                                      value_length);
  }
  return 0;
}

__envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatus
__envoy_dynamic_module_v1_event_http_response_headers(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr response_headers_map_ptr,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  {
    void* key = "new_key";
    size_t key_length = 7;
    void* value = "value";
    size_t value_length = 5;
    __envoy_dynamic_module_v1_http_set_response_header(response_headers_map_ptr, key, key_length,
                                                       value, value_length);
  }
  {
    void* key = "existing_key";
    size_t key_length = 12;
    void* value = "new_value";
    size_t value_length = 9;
    __envoy_dynamic_module_v1_http_set_response_header(response_headers_map_ptr, key, key_length,
                                                       value, value_length);
  }
  {
    void* key = "to_delete";
    size_t key_length = 9;
    void* value = NULL;
    size_t value_length = 0;
    __envoy_dynamic_module_v1_http_set_response_header(response_headers_map_ptr, key, key_length,
                                                       value, value_length);
  }
  {
    void* key = "existing_key_with_multiple_values";
    size_t key_length = 33;
    void* value = "unique_value";
    size_t value_length = 12;
    __envoy_dynamic_module_v1_http_set_response_header(response_headers_map_ptr, key, key_length,
                                                       value, value_length);
    return 0;
  }
}

__envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus
__envoy_dynamic_module_v1_event_http_request_body(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_HttpRequestBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

size_t __envoy_dynamic_module_v1_event_module_init(
    __envoy_dynamic_module_v1_type_ModuleConfigPtr config_ptr,
    __envoy_dynamic_module_v1_type_ModuleConfigSize config_size) {
  return 0;
}

size_t context = 0;

void* __envoy_dynamic_module_v1_event_http_context_init() {
  context = 999999;
  return &context;
}

__envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus
__envoy_dynamic_module_v1_event_http_response_body(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr,
    __envoy_dynamic_module_v1_type_HttpResponseBodyBufferPtr buffer,
    __envoy_dynamic_module_v1_type_EndOfStream end_of_stream) {
  return 0;
}

void __envoy_dynamic_module_v1_http_on_destroy(
    __envoy_dynamic_module_v1_type_EnvoyFilterPtr envoy_filter_ptr,
    __envoy_dynamic_module_v1_type_HttpContextPtr http_context_ptr) {}
