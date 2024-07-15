#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern size_t __envoy_dynamic_module_v1_http_get_request_body_buffer_slices_count(void* buffer);
extern size_t __envoy_dynamic_module_v1_http_get_response_body_buffer_slices_count(void* buffer);

extern void __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(
    void* buffer, size_t nth, void** result_buffer_ptr, size_t* result_buffer_length_ptr);
extern void __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(
    void* buffer, size_t nth, void** result_buffer_ptr, size_t* result_buffer_length_ptr);

int __envoy_dynamic_module_v1_http_on_request_body(void* envoy_filter_ptr, void* context_ptr,
                                                   void* buffer, size_t end_of_stream) {

  size_t slices_count = __envoy_dynamic_module_v1_http_get_request_body_buffer_slices_count(buffer);
  if (slices_count != 2) {
    printf("slice_count must be 2 but got %zu\n", slices_count);
    exit(9999);
  }

  for (size_t i = 0; i < slices_count; i++) {
    void* result_buffer_ptr;
    size_t result_buffer_length;
    __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(buffer, i, &result_buffer_ptr,
                                                                 &result_buffer_length);
    if (i == 0) {
      if (result_buffer_length != 5 ||
          strncmp(result_buffer_ptr, "hello", result_buffer_length) != 0) {
        printf("result[0] does not match: %s expected hello\n",
               strndup((char*)result_buffer_ptr, result_buffer_length));
        exit(9999);
      }
    } else if (i == 1) {
      if (result_buffer_length != 5 ||
          strncmp(result_buffer_ptr, "world", result_buffer_length) != 0) {
        printf("result[1] does not match: %s expected world\n",
               strndup((char*)result_buffer_ptr, result_buffer_length));
        exit(9999);
      }
    }
  }

  // Invalid n-th slice.
  void* result_buffer_ptr;
  size_t result_buffer_length;
  __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(buffer, 2, &result_buffer_ptr,
                                                               &result_buffer_length);
  if (result_buffer_length != 0 || result_buffer_ptr != NULL) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }
  return 0;
}

size_t __envoy_dynamic_module_v1_http_on_response_body(void* envoy_filter_ptr, void* context_ptr,
                                                       void* buffer, size_t end_of_stream) {
  size_t slices_count =
      __envoy_dynamic_module_v1_http_get_response_body_buffer_slices_count(buffer);
  if (slices_count != 2) {
    printf("slice_count must be 2 but got %zu\n", slices_count);
    exit(9999);
  }

  for (size_t i = 0; i < slices_count; i++) {
    void* result_buffer_ptr;
    size_t result_buffer_length;
    __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(buffer, i, &result_buffer_ptr,
                                                                  &result_buffer_length);
    if (i == 0) {
      if (result_buffer_length != 2 ||
          strncmp(result_buffer_ptr, "Go", result_buffer_length) != 0) {
        printf("result does not match: %s expected Go\n",
               strndup((char*)result_buffer_ptr, result_buffer_length));
        exit(9999);
      }
    } else if (i == 1) {
      if (result_buffer_length != 4 ||
          strncmp(result_buffer_ptr, "Rust", result_buffer_length) != 0) {
        printf("result does not match: %s expected Rust\n",
               strndup((char*)result_buffer_ptr, result_buffer_length));
        exit(9999);
      }
    }
  }

  // Invalid n-th slice.
  void* result_buffer_ptr;
  size_t result_buffer_length;
  __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(buffer, 2, &result_buffer_ptr,
                                                                &result_buffer_length);
  if (result_buffer_length != 0 || result_buffer_ptr != NULL) {
    printf("non-existent result_buffer_length: %zu\n", result_buffer_length);
    exit(9999);
  }
  return 0;
}

size_t __envoy_dynamic_module_v1_init(char* config) { return 0; }

size_t context = 0;

void* __envoy_dynamic_module_v1_http_context_init() {
  context = 999999;
  return &context;
}

size_t __envoy_dynamic_module_v1_http_on_request_headers(void* envoy_filter_ptr, void* context_ptr,
                                                         void* headers, size_t end_of_stream) {
  return 0;
}

int __envoy_dynamic_module_v1_http_on_response_headers(void* envoy_filter_ptr, void* context_ptr,
                                                       void* headers, size_t end_of_stream) {
  return 0;
}

size_t __envoy_dynamic_module_v1_http_on_destroy(void* envoy_filter_ptr, void* context_ptr,
                                                 void* buffer, size_t end_of_stream) {
  return 0;
}
