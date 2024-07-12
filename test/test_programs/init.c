#include <stdio.h>

int already_called = 0;

int __envoy_module_init(char* config) {
  printf("\t\tinit called with config: %s and already_called_ptr: %p, already_called: %d\n", config,
         &already_called, already_called);
  if (already_called == 1) {
    // This function should only be called once.
    return 1;
  }
  already_called = 1;

  printf("init called with config: %s and already_called_ptr: %p, already_called: %d\n", config,
         &already_called, already_called);

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

void* __envoy_module_http_stream_context_init() { return NULL; }
