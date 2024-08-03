
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

#include "envoy/server/filter_config.h"

#include "source/config.pb.h"
#include "source/extensions/dynamic_modules/dynamic_modules.h"
#include "source/extensions/dynamic_modules/abi/abi.h"

namespace Envoy {
namespace Extensions {
namespace DynamicModules {
namespace Http {

/**
 * A class to create http filter instances based on a dynamic module. This will be owned by multiple
 * filter instances.
 */
class HttpDynamicModule {
public:
  /**
   * Create a new module.
   * @param name the name of the module for debugging and logging purposes.
   * @param dynamic_module the dynamic module to load.
   */
  HttpDynamicModule(const std::string_view name, const std::string_view config,
                    Extensions::DynamicModules::DynamicModuleSharedPtr dynamic_module)
      : name_(name), dynamic_module_(dynamic_module) {
    initHttpFilter(config);
  };

  ~HttpDynamicModule();

  /**
   * Initialize the module.
   * @param config the configuration for the module.
   */
  void initHttpFilter(const std::string_view config);

  // Extracts the function types of each event hook.
  using envoy_dynamic_module_event_program_init =
      Envoy::Extensions::DynamicModules::function_type_t<
          decltype(&envoy_dynamic_module_event_program_init)>*;
  using envoy_dynamic_module_event_http_filter_init =
      Envoy::Extensions::DynamicModules::function_type_t<
          decltype(&envoy_dynamic_module_event_http_filter_init)>*;
  using envoy_dynamic_module_event_http_filter_destroy =
      Envoy::Extensions::DynamicModules::function_type_t<
          decltype(&envoy_dynamic_module_event_http_filter_destroy)>*;
  using envoy_dynamic_module_event_http_filter_instance_init =
      Envoy::Extensions::DynamicModules::function_type_t<
          decltype(&envoy_dynamic_module_event_http_filter_instance_init)>*;
  using envoy_dynamic_module_event_http_filter_instance_request_headers =
      Envoy::Extensions::DynamicModules::function_type_t<
          decltype(&envoy_dynamic_module_event_http_filter_instance_request_headers)>*;
  using envoy_dynamic_module_event_http_filter_instance_request_body =
      Envoy::Extensions::DynamicModules::function_type_t<
          decltype(&envoy_dynamic_module_event_http_filter_instance_request_body)>*;
  using envoy_dynamic_module_event_http_filter_instance_response_headers =
      Envoy::Extensions::DynamicModules::function_type_t<
          decltype(&envoy_dynamic_module_event_http_filter_instance_response_headers)>*;
  using envoy_dynamic_module_event_http_filter_instance_response_body =
      Envoy::Extensions::DynamicModules::function_type_t<
          decltype(&envoy_dynamic_module_event_http_filter_instance_response_body)>*;
  using envoy_dynamic_module_event_http_filter_instance_destroy =
      Envoy::Extensions::DynamicModules::function_type_t<
          decltype(&envoy_dynamic_module_event_http_filter_instance_destroy)>*;

  // The event hooks for the module.
  envoy_dynamic_module_event_program_init envoy_dynamic_module_event_program_init_ = nullptr;
  envoy_dynamic_module_event_http_filter_init envoy_dynamic_module_event_http_filter_init_ =
      nullptr;
  envoy_dynamic_module_event_http_filter_destroy envoy_dynamic_module_event_http_filter_destroy_ =
      nullptr;
  envoy_dynamic_module_event_http_filter_instance_init
      envoy_dynamic_module_event_http_filter_instance_init_ = nullptr;
  envoy_dynamic_module_event_http_filter_instance_request_headers
      envoy_dynamic_module_event_http_filter_instance_request_headers_ = nullptr;
  envoy_dynamic_module_event_http_filter_instance_request_body
      envoy_dynamic_module_event_http_filter_instance_request_body_ = nullptr;
  envoy_dynamic_module_event_http_filter_instance_response_headers
      envoy_dynamic_module_event_http_filter_instance_response_headers_ = nullptr;
  envoy_dynamic_module_event_http_filter_instance_response_body
      envoy_dynamic_module_event_http_filter_instance_response_body_ = nullptr;
  envoy_dynamic_module_event_http_filter_instance_destroy
      envoy_dynamic_module_event_http_filter_instance_destroy_ = nullptr;

  // The in-module http filter for the module.
  void* http_filter_ = nullptr;

  // The name of the module passed in the constructor.
  const std::string name_;

  // The handle for the module.
  Extensions::DynamicModules::DynamicModuleSharedPtr dynamic_module_ = nullptr;
};

using HttpDynamicModuleSharedPtr = std::shared_ptr<HttpDynamicModule>;

} // namespace Http
} // namespace DynamicModules
} // namespace Extensions
} // namespace Envoy
