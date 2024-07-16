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
  ASSERT(handle_ != nullptr);
  dlclose(handle_);
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
  handle_ = dlopen(copied_file_path_.c_str(), RTLD_LOCAL | RTLD_LAZY);
  if (!handle_) {
    throw EnvoyException(fmt::format("cannot load : {} error: {}", copied_file_path_, dlerror()));
  }

  // Initialize the module.
  initModule(config);
}

#define RESOLVE_SYMBOL_OR_THROW(symbol_type)                                                       \
  symbol_type##_ = reinterpret_cast<symbol_type>(dlsym(handle_, #symbol_type));                    \
  if (symbol_type##_ == nullptr) {                                                                 \
    throw EnvoyException(                                                                          \
        fmt::format("cannot resolve symbol: {} error: {}", #symbol_type, dlerror()));              \
  }

void DynamicModule::initModule(const std::string& config) {
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_module_init);
  const int result = __envoy_dynamic_module_v1_event_module_init_(config.data(), config.size());
  if (result != 0) {
    throw EnvoyException(
        fmt::format("init function in {} failed with result {}", copied_file_path_, result));
  }
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_context_init);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_request_headers);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_request_body);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_response_headers);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_response_body);
  RESOLVE_SYMBOL_OR_THROW(__envoy_dynamic_module_v1_event_http_destroy);
}

#undef RESOLVE_SYMBOL_OR_THROW

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
