
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
   * Get the handle for testing.
   * @return the handle.
   */
  void* handleForTesting() { return handle_; }

  __envoy_dynamic_module_v1_event_module_init __envoy_dynamic_module_v1_event_module_init_ =
      nullptr;
  __envoy_dynamic_module_v1_event_http_context_init
      __envoy_dynamic_module_v1_event_http_context_init_ = nullptr;
  __envoy_dynamic_module_v1_event_http_request_headers
      __envoy_dynamic_module_v1_event_http_request_headers_ = nullptr;
  __envoy_dynamic_module_v1_event_http_request_body
      __envoy_dynamic_module_v1_event_http_request_body_ = nullptr;
  __envoy_dynamic_module_v1_event_http_response_headers
      __envoy_dynamic_module_v1_event_http_response_headers_ = nullptr;
  __envoy_dynamic_module_v1_event_http_response_body
      __envoy_dynamic_module_v1_event_http_response_body_ = nullptr;
  __envoy_dynamic_module_v1_event_http_destroy __envoy_dynamic_module_v1_event_http_destroy_ =
      nullptr;

private:
  // The raw dlopen handle.
  void* handle_ = nullptr;
  // The path to the copied object file.
  std::string copied_file_path_;
};

using DynamicModuleSharedPtr = std::shared_ptr<DynamicModule>;

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
