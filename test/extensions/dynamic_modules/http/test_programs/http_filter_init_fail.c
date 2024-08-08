
#include "source/extensions/dynamic_modules/abi/abi.h"

size_t envoy_dynamic_module_on_program_init() { return 0; }

envoy_dynamic_module_type_HttpFilterPtr envoy_dynamic_module_on_http_filter_init(
    envoy_dynamic_module_type_HttpFilterConfigPtr config_ptr,
    envoy_dynamic_module_type_HttpFilterConfigSize config_size) {
  return 0;
}