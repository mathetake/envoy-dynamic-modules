#include "source/extensions/dynamic_modules/abi/abi.h"

int getSomeVariable() {
  static int some_variable = 0;
  some_variable++;
  return some_variable;
}

size_t envoy_dynamic_module_event_program_init() { return 0; }