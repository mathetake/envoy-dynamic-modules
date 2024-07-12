#include <stdio.h>

int __envoy_module_init(char* config) { return 0; }

int context = 0;

void* __envoy_module_http_stream_context_init() {
  context = 999999;
  return &context;
}
