#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern size_t
__envoy_dynamic_module_v1_http_get_request_header_value(void* headers, void* key, size_t key_length,
                                                        void** result_buffer_ptr,
                                                        size_t* result_buffer_length_ptr);

extern size_t __envoy_dynamic_module_v1_http_get_response_header_value(
    void* headers, void* key, size_t key_length, void** result_buffer_ptr,
    size_t* result_buffer_length_ptr);

size_t __envoy_dynamic_module_v1_http_on_request_headers(void* envoy_filter_ptr, void* context_ptr,
                                                         void* headers, size_t end_of_stream) {
  // Get the value of the header "key" from the headers.
  void* key = "key";
  size_t key_length = 3;
  void* result_buffer_ptr;
  size_t result_buffer_length;
  __envoy_dynamic_module_v1_http_get_request_header_value(
      headers, key, key_length, &result_buffer_ptr, &result_buffer_length);
  // Check if the header value is "value".
  void* value = "value";
  if (result_buffer_length != 5 || strncmp(result_buffer_ptr, value, result_buffer_length) != 0) {
    printf("result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  // Get the non-existent header "non-existent-key" from the headers.
  key = "non-existent-key";
  key_length = 15;
  __envoy_dynamic_module_v1_http_get_request_header_value(
      headers, key, key_length, &result_buffer_ptr, &result_buffer_length);
  // Check if the header value is NULL.
  if (result_buffer_length != 0 || result_buffer_ptr != NULL) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  printf("OK\n");
  return 0;
}

int __envoy_dynamic_module_v1_http_on_response_headers(void* envoy_filter_ptr, void* context_ptr,
                                                       void* headers, size_t end_of_stream) {
  // Get the value of the header "foo" from the headers.
  void* key = "foo";
  size_t key_length = 3;
  void* result_buffer_ptr;
  size_t result_buffer_length;
  __envoy_dynamic_module_v1_http_get_response_header_value(
      headers, key, key_length, &result_buffer_ptr, &result_buffer_length);
  // Check if the header value is "value".
  void* value = "bar";
  if (result_buffer_length != 3 || strncmp(result_buffer_ptr, value, result_buffer_length) != 0) {
    printf("result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  // Get the non-existent header "non-existent-key" from the headers.
  key = "non-existent-key";
  key_length = 15;
  __envoy_dynamic_module_v1_http_get_response_header_value(
      headers, key, key_length, &result_buffer_ptr, &result_buffer_length);
  // Check if the header value is NULL.
  if (result_buffer_length != 0 || result_buffer_ptr != NULL) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }

  printf("OK\n");
  return 0;
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
