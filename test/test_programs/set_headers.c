#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void __envoy_dynamic_module_v1_http_set_request_header(void* headers, void* key, size_t key_length,
                                                       void* value, size_t value_length);

void __envoy_dynamic_module_v1_http_set_response_header(void* headers, void* key, size_t key_length,
                                                        void* value, size_t value_length);

size_t __envoy_dynamic_module_v1_http_on_request_headers(void* envoy_filter_ptr, void* context_ptr,
                                                         void* headers, size_t end_of_stream) {
  {
    void* key = "new_key";
    size_t key_length = 7;
    void* value = "value";
    size_t value_length = 5;
    __envoy_dynamic_module_v1_http_set_request_header(headers, key, key_length, value,
                                                      value_length);
  }
  {
    void* key = "existing_key";
    size_t key_length = 12;
    void* value = "new_value";
    size_t value_length = 9;
    __envoy_dynamic_module_v1_http_set_request_header(headers, key, key_length, value,
                                                      value_length);
  }
  {
    void* key = "to_delete";
    size_t key_length = 9;
    void* value = NULL;
    size_t value_length = 0;
    __envoy_dynamic_module_v1_http_set_request_header(headers, key, key_length, value,
                                                      value_length);
  }
  {
    void* key = "existing_key_with_multiple_values";
    size_t key_length = 32;
    void* value = "unique_value";
    size_t value_length = 12;
    __envoy_dynamic_module_v1_http_set_request_header(headers, key, key_length, value,
                                                      value_length);
  }
  return 0;
}

int __envoy_dynamic_module_v1_http_on_response_headers(void* envoy_filter_ptr, void* context_ptr,
                                                       void* headers, size_t end_of_stream) {
  {
    void* key = "new_key";
    size_t key_length = 7;
    void* value = "value";
    size_t value_length = 5;
    __envoy_dynamic_module_v1_http_set_response_header(headers, key, key_length, value,
                                                       value_length);
  }
  {
    void* key = "existing_key";
    size_t key_length = 12;
    void* value = "new_value";
    size_t value_length = 9;
    __envoy_dynamic_module_v1_http_set_response_header(headers, key, key_length, value,
                                                       value_length);
  }
  {
    void* key = "to_delete";
    size_t key_length = 9;
    void* value = NULL;
    size_t value_length = 0;
    __envoy_dynamic_module_v1_http_set_response_header(headers, key, key_length, value,
                                                       value_length);
  }
  {
    void* key = "existing_key_with_multiple_values";
    size_t key_length = 33;
    void* value = "unique_value";
    size_t value_length = 12;
    __envoy_dynamic_module_v1_http_set_response_header(headers, key, key_length, value,
                                                       value_length);
    return 0;
  }
}

int __envoy_dynamic_module_v1_http_on_request_body(void* envoy_filter_ptr, void* context_ptr,
                                                   void* buffer, size_t end_of_stream) {
  return 0;
}

size_t __envoy_dynamic_module_v1_init(char* config) { return 0; }

size_t context = 0;

void* __envoy_dynamic_module_v1_http_context_init() {
  context = 999999;
  return &context;
}

size_t __envoy_dynamic_module_v1_http_on_response_body(void* envoy_filter_ptr, void* context_ptr,
                                                       void* buffer, size_t end_of_stream) {
  return 0;
}

size_t __envoy_dynamic_module_v1_http_on_destroy(void* envoy_filter_ptr, void* context_ptr,
                                                 void* buffer, size_t end_of_stream) {
  return 0;
}
