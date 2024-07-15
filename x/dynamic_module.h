
#pragma once

#include <cstddef>
#include <optional>
#include <string>

#include "envoy/server/filter_config.h"

#include "x/config.pb.h"
#include "x/abi.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

// Generic template function to resolve symbols from a dynamic module, or throw an exception if the
// symbol is not found.
template <typename SymbolType, const char* symbolName>
SymbolType resolveSymbolOrThrow(void* handler) {
  static_assert(symbolName != nullptr, "Symbol name must not be null.");
  void* symbol = dlsym(handler, symbolName);
  if (!symbol) {
    throw EnvoyException(fmt::format("cannot find symbol: {} error: {}", symbolName, dlerror()));
  }
  return reinterpret_cast<SymbolType>(symbol);
}

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
  ABI::EnvoyModuleHttpContextInitSig envoyModuleHttpContextInit() {
    ASSERT(envoy_module_http_context_init_ != nullptr);
    return envoy_module_http_context_init_;
  }

  /**
   * Get the function pointer to the module's http on request headers function.
   * @return the function pointer of Symbols::EnvoyModuleHttpOnRequestHeadersSig.
   */
  ABI::EnvoyModuleHttpOnRequestHeadersSig envoyModuleHttpOnRequestHeaders() {
    ASSERT(envoy_module_http_on_request_headers_ != nullptr);
    return envoy_module_http_on_request_headers_;
  }

  /**
   * Get the function pointer to the module's http on request body function.
   * @return the function pointer of Symbols::EnvoyModuleHttpOnRequestBodySig.
   */
  ABI::EnvoyModuleHttpOnRequestBodySig envoyModuleHttpOnRequestBody() {
    ASSERT(envoy_module_http_on_request_body_ != nullptr);
    return envoy_module_http_on_request_body_;
  }

  /**
   * Get the function pointer to the module's http on response headers function.
   * @return the function pointer of Symbols::EnvoyModuleHttpOnResponseHeadersSig.
   */
  ABI::EnvoyModuleHttpOnResponseHeadersSig envoyModuleHttpOnResponseHeaders() {
    ASSERT(envoy_module_http_on_response_headers_ != nullptr);
    return envoy_module_http_on_response_headers_;
  }

  /**
   * Get the function pointer to the module's http on response body function.
   * @return the function pointer of Symbols::EnvoyModuleHttpOnResponseBodySig.
   */

  ABI::EnvoyModuleHttpOnResponseBodySig envoyModuleHttpOnResponseBody() {
    ASSERT(envoy_module_http_on_response_body_ != nullptr);
    return envoy_module_http_on_response_body_;
  }

  /**
   * Get the function pointer to the module's http on destroy function.
   * @return the function pointer of Symbols::EnvoyModuleHttpOnDestroySig.
   */
  ABI::EnvoyModuleHttpOnDestroySig envoyModuleHttpOnDestroy() {
    ASSERT(envoy_module_http_on_destroy_ != nullptr);
    return envoy_module_http_on_destroy_;
  }

  void* handlerForTesting() { return handler_; }

private:
  // The raw dlopen handler.
  void* handler_ = nullptr;
  // The path to the copied object file.
  std::string copied_file_path_;

  ABI::EnvoyModuleHttpContextInitSig envoy_module_http_context_init_ = nullptr;
  ABI::EnvoyModuleHttpOnRequestHeadersSig envoy_module_http_on_request_headers_ = nullptr;
  ABI::EnvoyModuleHttpOnRequestBodySig envoy_module_http_on_request_body_ = nullptr;
  ABI::EnvoyModuleHttpOnResponseHeadersSig envoy_module_http_on_response_headers_ = nullptr;
  ABI::EnvoyModuleHttpOnResponseBodySig envoy_module_http_on_response_body_ = nullptr;
  ABI::EnvoyModuleHttpOnDestroySig envoy_module_http_on_destroy_ = nullptr;
};

using DynamicModuleSharedPtr = std::shared_ptr<DynamicModule>;

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
