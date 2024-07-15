#include <stddef.h>
#include <stdio.h>
#include <string.h>

int already_called = 0;

int __envoy_dynamic_module_v1_init(char* config, size_t config_size) {
  printf("\t\tinit called with config: %s and already_called_ptr: %p, already_called: %d\n", config,
         &already_called, already_called);
  if (already_called == 1) {
    // This function should only be called once.
    return 1;
  }
  already_called = 1;

  printf("init called with config: %s and already_called_ptr: %p, already_called: %d\n", config,
         &already_called, already_called);

  if (config_size != 6) {
    return 1;
  }

  // Checks if the config is equals to "config"
  if (strcmp(config, "config") != 0) {
    return 1;
  }
  // Replaces the config with "11111" via for loop.
  for (int i = 0; i < 6; i++) {
    config[i] = '1';
  }
  return 0;
}

void* __envoy_dynamic_module_v1_http_context_init() { return NULL; }

int __envoy_dynamic_module_v1_http_on_request_headers(void* envoy_filter_ptr, void* context_ptr,
                                                      void* headers, int end_of_stream) {
  return 0;
}
int __envoy_dynamic_module_v1_http_on_request_body(void* envoy_filter_ptr, void* context_ptr,
                                                   void* buffer, int end_of_stream) {
  return 0;
}
int __envoy_dynamic_module_v1_http_on_response_headers(void* envoy_filter_ptr, void* context_ptr,
                                                       void* headers, int end_of_stream) {
  return 0;
}
int __envoy_dynamic_module_v1_http_on_response_body(void* envoy_filter_ptr, void* context_ptr,
                                                    void* buffer, int end_of_stream) {
  return 0;
}
int __envoy_dynamic_module_v1_http_on_destroy(void* envoy_filter_ptr, void* context_ptr,
                                              void* buffer, int end_of_stream) {
  return 0;
}