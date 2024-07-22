
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

#include "envoy/server/filter_config.h"

#include "x/config.pb.h"
#include "abi.h"

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
   * @param name the name of the module for debugging and logging purposes.
   * @param object_file_location the location of the object file.
   * @param config the configuration for the module.
   * @param do_not_close if true, the module will not be closed when the object is destroyed.
   */
  DynamicModule(const std::string& name, const std::string_view object_file_location,
                const std::string& config, const bool do_not_close)
      : name_(name) {
    initModule(object_file_location, config, do_not_close);
  }

  ~DynamicModule();

  /**
   * Initialize the module on the local path.
   * @param location the location of the object file.
   * @param config the configuration for the module.
   * @param do_not_close if true, the module will not be closed when the object is destroyed.
   */
  void initModule(const std::string_view object_file_location, const std::string& config,
                  const bool do_not_close);

  /**
   * Initialize the module.
   * @param config the configuration for the module.
   */
  void initHttpFilter(const std::string& config);

  /**
   * Get the handle for testing.
   * @return the handle.
   */
  void* handleForTesting() { return handle_; }

#define DECLARE_EVENT_HOOK(name) name name##_ = nullptr;
  DECLARE_EVENT_HOOK(__envoy_dynamic_module_v1_event_program_init)
  DECLARE_EVENT_HOOK(__envoy_dynamic_module_v1_event_http_filter_init)
  DECLARE_EVENT_HOOK(__envoy_dynamic_module_v1_event_http_filter_destroy)
  DECLARE_EVENT_HOOK(__envoy_dynamic_module_v1_event_http_filter_instance_init)
  DECLARE_EVENT_HOOK(__envoy_dynamic_module_v1_event_http_filter_instance_request_headers)
  DECLARE_EVENT_HOOK(__envoy_dynamic_module_v1_event_http_filter_instance_request_body)
  DECLARE_EVENT_HOOK(__envoy_dynamic_module_v1_event_http_filter_instance_response_headers)
  DECLARE_EVENT_HOOK(__envoy_dynamic_module_v1_event_http_filter_instance_response_body)
  DECLARE_EVENT_HOOK(__envoy_dynamic_module_v1_event_http_filter_instance_destroy)
#undef DECLARE_EVENT_HOOK

  // The in-memory context for the module.
  void* module_ctx_ = nullptr;

  // The name of the module passed in the constructor.
  const std::string name_;

private:
  // The raw dlopen handle.
  void* handle_ = nullptr;
  std::string file_path_for_cleanup_;
};

using DynamicModuleSharedPtr = std::shared_ptr<DynamicModule>;

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
