
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

#include "envoy/server/filter_config.h"

#include "x/config.pb.h"
#include "abi/abi.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

// Templates to get the function type from a function declaration in abi.h.
template <typename T> struct function_traits;
template <typename Ret, typename... Args> struct function_traits<Ret (*)(Args...)> {
  using type = Ret(Args...);
};
template <typename T> using function_type_t = typename function_traits<T>::type;

/**
 * A class to manage a dynamic module. This will be owned by multiple filter instances.
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

  // Extracts the function types of each event hook.
  using envoy_dynamic_module_event_program_init =
      function_type_t<decltype(&envoy_dynamic_module_event_program_init)>*;
  using envoy_dynamic_module_event_http_filter_init =
      function_type_t<decltype(&envoy_dynamic_module_event_http_filter_init)>*;
  using envoy_dynamic_module_event_http_filter_destroy =
      function_type_t<decltype(&envoy_dynamic_module_event_http_filter_destroy)>*;
  using envoy_dynamic_module_event_http_filter_instance_init =
      function_type_t<decltype(&envoy_dynamic_module_event_http_filter_instance_init)>*;
  using envoy_dynamic_module_event_http_filter_instance_request_headers =
      function_type_t<decltype(&envoy_dynamic_module_event_http_filter_instance_request_headers)>*;
  using envoy_dynamic_module_event_http_filter_instance_request_body =
      function_type_t<decltype(&envoy_dynamic_module_event_http_filter_instance_request_body)>*;
  using envoy_dynamic_module_event_http_filter_instance_response_headers =
      function_type_t<decltype(&envoy_dynamic_module_event_http_filter_instance_response_headers)>*;
  using envoy_dynamic_module_event_http_filter_instance_response_body =
      function_type_t<decltype(&envoy_dynamic_module_event_http_filter_instance_response_body)>*;
  using envoy_dynamic_module_event_http_filter_instance_destroy =
      function_type_t<decltype(&envoy_dynamic_module_event_http_filter_instance_destroy)>*;

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

private:
  // The raw dlopen handle.
  void* handle_ = nullptr;
};

using DynamicModuleSharedPtr = std::shared_ptr<DynamicModule>;

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
