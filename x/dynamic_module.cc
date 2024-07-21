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
  std::filesystem::remove(file_path_for_cleanup_);
}

void DynamicModule::initModuleOnLocal(const ObjectFileLocationFilePath& location,
                                      const std::string& config, const std::string& uuid) {
  // To reload a module, we need to create a hard copy of the file and load that.
  const std::filesystem::path file_path_absolute = std::filesystem::absolute(location.file_path);
  file_path_for_cleanup_ = file_path_absolute.string() + "." + uuid;

  std::filesystem::copy(file_path_absolute, file_path_for_cleanup_,
                        std::filesystem::copy_options::recursive);

  // Load the module with RTLD_LOCAL to avoid symbol conflicts with other modules.
  handle_ = dlopen(file_path_for_cleanup_.c_str(), RTLD_LOCAL | RTLD_LAZY);
  if (!handle_) {
    throw EnvoyException(fmt::format("cannot load : {} error: {}", name_, dlerror()));
  }

  // Initialize the module.
  initModule(config);
}

void DynamicModule::initModuleOnInlineBytes(const ObjectFileLocationInlineBytes& location,
                                            const std::string& config, const std::string& uuid) {

  // To load a module from inline bytes, we need to create a temporary file and load that.
  const std::filesystem::path tmpdir = std::filesystem::temp_directory_path();
  file_path_for_cleanup_ = tmpdir / ("lib" + uuid + ".so");

  // Copy the bytes to the file.
  std::ofstream file(file_path_for_cleanup_, std::ios::binary);
  file.write(location.inline_bytes.data(), location.inline_bytes.size());
  file.close();

  // Load the module with RTLD_LOCAL to avoid symbol conflicts with other modules.
  handle_ = dlopen(file_path_for_cleanup_.c_str(), RTLD_LOCAL | RTLD_LAZY);
  if (!handle_) {
    throw EnvoyException(fmt::format("cannot load : {} error: {}", name_, dlerror()));
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
  module_ctx_ =
      __envoy_dynamic_module_v1_event_module_init_(const_cast<char*>(config.data()), config.size());
  if (module_ctx_ == nullptr) {
    throw EnvoyException(fmt::format("init function in {} failed", name_));
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
