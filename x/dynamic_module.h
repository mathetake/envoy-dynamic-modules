
#pragma once

#include <cstddef>
#include <string>

#include "envoy/server/filter_config.h"

#include "x/config.pb.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

namespace Symbols {

// __envoy_module_init is called by the main thread when the module is loaded exactly once per
// module. The function should return 0 on success and non-zero on failure.
constexpr char __envoy_module_init[] = "__envoy_module_init";
using EnvoyModuleInit = int (*)(const char*);

// __envoy_module_http_stream_context_init is called by any worker thread when a new stream is
// created. That means that the function should be thread-safe.
//
// The function should return a pointer to a new instance of the filter or nullptr on failure.
// The lifetime of the returned pointer should be managed by the dynamic module.
constexpr char __envoy_module_http_stream_context_init[] =
    "__envoy_module_http_stream_context_init";
using EnvoyModuleHttpStreamContextInit = void* (*)();

// Generic template function to resolve symbols.
template <typename SymbolType, const char* symbolName> SymbolType resolveSymbol(void* handler) {
  static_assert(symbolName != nullptr, "Symbol name must not be null.");
  void* symbol = dlsym(handler, symbolName);
  if (!symbol) {
    return nullptr;
  }
  return reinterpret_cast<SymbolType>(symbol);
}

} // namespace Symbols

/**
 * A class to manage a dynamic module. This will be owned by multiple filters.
 */
class DynamicModule {
public:
  /**
   * Create a new module.
   * @param file_path the path to the module.
   * @param config the configuration for the module.
   * @param random_uuid a random uuid to avoid conflicts with other modules.
   */
  DynamicModule(const std::string& file_path, const std::string& config,
                const std::string&& random_uuid);

  ~DynamicModule();

  /**
   * Initialize the module.
   * @param config the configuration for the module.
   */
  void initModule(const std::string& config);

  /**
   * Get the function pointer to the module's http stream init function.
   * @return the function pointer of Symbols::EnvoyModuleHttpStreamInit.
   */
  Symbols::EnvoyModuleHttpStreamContextInit envoyModuleHttpStreamContextInit() {
    ASSERT(fn_envoy_module_http_stream_context_init_ != nullptr);
    return fn_envoy_module_http_stream_context_init_;
  }

private:
  void* handler_ = nullptr;
  std::string symlink_path_;

  // Symbols.
  Symbols::EnvoyModuleHttpStreamContextInit fn_envoy_module_http_stream_context_init_ = nullptr;
};

using DynamicModuleSharedPtr = std::shared_ptr<DynamicModule>;

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
