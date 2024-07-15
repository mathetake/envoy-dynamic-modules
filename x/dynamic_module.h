
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
   * @return the function pointer of __envoy_dynamic_module_v1_init.
   */
  __envoy_dynamic_module_v1_http_context_init envoyModuleHttpContextInit() {
    ASSERT(envoy_dynamic_module_v1_http_context_init_ != nullptr);
    return envoy_dynamic_module_v1_http_context_init_;
  }

  /**
   * Get the function pointer to the module's http on request headers function.
   * @return the function pointer of __envoy_dynamic_module_v1_http_on_request_headers.
   */
  __envoy_dynamic_module_v1_http_on_request_headers envoyModuleHttpOnRequestHeaders() {
    ASSERT(envoy_dynamic_module_v1_http_on_request_headers_ != nullptr);
    return envoy_dynamic_module_v1_http_on_request_headers_;
  }

  /**
   * Get the function pointer to the module's http on request body function.
   * @return the function pointer of __envoy_dynamic_module_v1_http_on_request_body.
   */
  __envoy_dynamic_module_v1_http_on_request_body envoyModuleHttpOnRequestBody() {
    ASSERT(envoy_dynamic_module_v1_http_on_request_body_ != nullptr);
    return envoy_dynamic_module_v1_http_on_request_body_;
  }

  /**
   * Get the function pointer to the module's http on response headers function.
   * @return the function pointer of __envoy_dynamic_module_v1_http_on_response_headers.
   */
  __envoy_dynamic_module_v1_http_on_response_headers envoyModuleHttpOnResponseHeaders() {
    ASSERT(envoy_dynamic_module_v1_http_on_response_headers_ != nullptr);
    return envoy_dynamic_module_v1_http_on_response_headers_;
  }

  /**
   * Get the function pointer to the module's http on response body function.
   * @return the function pointer of __envoy_dynamic_module_v1_http_on_response_body.
   */

  __envoy_dynamic_module_v1_http_on_response_body envoyModuleHttpOnResponseBody() {
    ASSERT(envoy_dynamic_module_v1_http_on_response_body_ != nullptr);
    return envoy_dynamic_module_v1_http_on_response_body_;
  }

  /**
   * Get the function pointer to the module's http on destroy function.
   * @return the function pointer of __envoy_dynamic_module_v1_http_on_destroy.
   */
  __envoy_dynamic_module_v1_http_on_destroy envoyModuleHttpOnDestroy() {
    ASSERT(envoy_dynamic_module_v1_http_on_destroy_ != nullptr);
    return envoy_dynamic_module_v1_http_on_destroy_;
  }

  void* handlerForTesting() { return handler_; }

private:
  // The raw dlopen handler.
  void* handler_ = nullptr;
  // The path to the copied object file.
  std::string copied_file_path_;

  __envoy_dynamic_module_v1_http_context_init envoy_dynamic_module_v1_http_context_init_ = nullptr;
  __envoy_dynamic_module_v1_http_on_request_headers
      envoy_dynamic_module_v1_http_on_request_headers_ = nullptr;
  __envoy_dynamic_module_v1_http_on_request_body envoy_dynamic_module_v1_http_on_request_body_ =
      nullptr;
  __envoy_dynamic_module_v1_http_on_response_headers
      envoy_dynamic_module_v1_http_on_response_headers_ = nullptr;
  __envoy_dynamic_module_v1_http_on_response_body envoy_dynamic_module_v1_http_on_response_body_ =
      nullptr;
  __envoy_dynamic_module_v1_http_on_destroy envoy_dynamic_module_v1_http_on_destroy_ = nullptr;
};

using DynamicModuleSharedPtr = std::shared_ptr<DynamicModule>;

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
