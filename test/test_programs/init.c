#include <stdio.h>

int __envoy_init(char* config) {
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
