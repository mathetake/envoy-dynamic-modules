#include <stdio.h>

int __envoy_module_init(char* config) { return 0; }

int context = 0;

void* __envoy_module_http_context_init() {
  context = 999999;
  return &context;
}

int __envoy_module_http_on_request_headers(void* envoy_filter_ptr, void* context_ptr) { return 0; }
int __envoy_module_http_on_request_body(void* envoy_filter_ptr, void* context_ptr) { return 0; }
int __envoy_module_http_on_response_headers(void* envoy_filter_ptr, void* context_ptr) { return 0; }
int __envoy_module_http_on_response_body(void* envoy_filter_ptr, void* context_ptr) { return 0; }
int __envoy_module_http_on_destroy(void* envoy_filter_ptr, void* context_ptr) { return 0; }