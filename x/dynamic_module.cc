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
  ASSERT(handler_ != nullptr);
  dlclose(handler_);
  std::filesystem::remove(copied_file_path_);
}

DynamicModule::DynamicModule(const std::string& file_path, const std::string& config,
                             const std::string&& uuid) {
  // dlopen does only return a new handle if the file is not already loaded. If the inode
  // of the file is the same as a file that is already loaded, dlopen will return the same one.
  //
  // To reload a module, we need to create a hard copy of the file and load that.
  const std::filesystem::path file_path_absolute = std::filesystem::absolute(file_path);
  copied_file_path_ = file_path_absolute.string() + "." + uuid;

  std::filesystem::copy(file_path_absolute, copied_file_path_,
                        std::filesystem::copy_options::recursive);

  // Load the module with RTLD_LOCAL to avoid symbol conflicts with other modules.
  handler_ = dlopen(copied_file_path_.c_str(), RTLD_LOCAL | RTLD_LAZY);
  if (!handler_) {
    throw EnvoyException(fmt::format("cannot load : {} error: {}", copied_file_path_, dlerror()));
  }

  // Initialize the module.
  initModule(config);
}

// Generic template function to resolve symbols from a dynamic module, or throw an exception if the
// symbol is not found.
#define RESOLVE_SYMBOL_OR_THROW(symbol_type, symbol_name, symbol_var)                              \
  symbol_var = reinterpret_cast<symbol_type>(dlsym(handler_, symbol_name));                        \
  if (symbol_var == nullptr) {                                                                     \
    throw EnvoyException(                                                                          \
        fmt::format("cannot resolve symbol: {} error: {}", symbol_name, dlerror()));               \
  }

void DynamicModule::initModule(const std::string& config) {
  __envoy_dynamic_module_v1_init init = nullptr;
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_init, "__envoy_dynamic_module_v1_init", init);
  const int result = init(config.data());
  if (result != 0) {
    throw EnvoyException(
        fmt::format("init function in {} failed with result {}", copied_file_path_, result));
  }

  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_http_context_init,
                          "__envoy_dynamic_module_v1_http_context_init",
                          envoy_dynamic_module_v1_http_context_init_);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_http_on_request_headers,
                          "__envoy_dynamic_module_v1_http_on_request_headers",
                          envoy_dynamic_module_v1_http_on_request_headers_);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_http_on_request_body,
                          "__envoy_dynamic_module_v1_http_on_request_body",
                          envoy_dynamic_module_v1_http_on_request_body_);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_http_on_response_headers,
                          "__envoy_dynamic_module_v1_http_on_response_headers",
                          envoy_dynamic_module_v1_http_on_response_headers_);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_http_on_response_body,
                          "__envoy_dynamic_module_v1_http_on_response_body",
                          envoy_dynamic_module_v1_http_on_response_body_);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_http_on_destroy,
                          "__envoy_dynamic_module_v1_http_on_destroy",
                          envoy_dynamic_module_v1_http_on_destroy_);
}

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
