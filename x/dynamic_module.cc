#include <filesystem>
#include <string>

#include "x/dynamic_module.h"

#include <dlfcn.h>

#include "source/common/common/assert.h"
#include "envoy/common/exception.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

DynamicModule::~DynamicModule() {
  __envoy_dynamic_module_v1_event_http_filter_destroy_(module_ctx_);
  ASSERT(handle_ != nullptr);
  dlclose(handle_);
  if (!file_path_for_cleanup_.empty()) {
    std::filesystem::remove(file_path_for_cleanup_);
  }
}

#define RESOLVE_SYMBOL_OR_THROW(symbol_type)                                                       \
  symbol_type##_ = reinterpret_cast<symbol_type>(dlsym(handle_, #symbol_type));                    \
  if (symbol_type##_ == nullptr) {                                                                 \
    throw EnvoyException(                                                                          \
        fmt::format("cannot resolve symbol: {} error: {}", #symbol_type, dlerror()));              \
  }

void DynamicModule::initModuleOnLocal(const std::string_view location, const std::string& config) {
  const std::filesystem::path file_path_absolute = std::filesystem::absolute(location);

  // Test if the shared object is already openend.
  handle_ = dlopen(file_path_absolute.c_str(), RTLD_NOLOAD | RTLD_LOCAL | RTLD_LAZY);
  if (!handle_) {
    // If the shared object is not opened, open it. Note that this runs on main thread.
    handle_ = dlopen(file_path_absolute.c_str(), RTLD_LOCAL | RTLD_LAZY);
    if (!handle_) {
      throw EnvoyException(fmt::format("cannot load : {} error: {}", name_, dlerror()));
    }
    // Calls the program init function.
    RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_program_init);
    if (__envoy_dynamic_module_v1_event_program_init_() != 0) {
      throw EnvoyException(fmt::format("program init in {} failed", name_));
    }
  }

  // Load the module with RTLD_LOCAL to avoid symbol conflicts with other modules.

  // Initialize the module.
  initHttpFilter(config);
}

void DynamicModule::initHttpFilter(const std::string& config) {
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_filter_init);
  module_ctx_ = __envoy_dynamic_module_v1_event_http_filter_init_(const_cast<char*>(config.data()),
                                                                  config.size());
  if (module_ctx_ == nullptr) {
    throw EnvoyException(fmt::format("http filter init in {} failed", name_));
  }
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_filter_destroy);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_filter_instance_init);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_filter_instance_request_headers);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_filter_instance_request_body);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_filter_instance_response_headers);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_filter_instance_response_body);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_filter_instance_destroy);
}

#undef RESOLVE_SYMBOL_OR_THROW

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
