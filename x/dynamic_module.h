
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

struct ObjectFileLocationFilePath {
  std::string_view file_path;
};
struct ObjectFileLocationInlineBytes {
  std::string_view inline_bytes;
};
/**
 * The location of the object file.
 * This can be a file path or inline bytes.
 */
using ObjectFileLocation = std::variant<ObjectFileLocationFilePath, ObjectFileLocationInlineBytes>;

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
   * @param uuid a random uuid to avoid conflicts with other modules.
   */
  DynamicModule(const std::string& name, const ObjectFileLocation& object_file_location,
                const std::string& config, const std::string& uuid)
      : name_(name) {
    if (std::holds_alternative<ObjectFileLocationFilePath>(object_file_location)) {
      initModuleOnLocal(std::get<ObjectFileLocationFilePath>(object_file_location), config, uuid);
    } else {
      initModuleOnInlineBytes(std::get<ObjectFileLocationInlineBytes>(object_file_location), config,
                              uuid);
    }
  }

  ~DynamicModule();

  /**
   * Initialize the module on the local path.
   * @param location the location of the object file.
   * @param config the configuration for the module.
   * @param uuid a random uuid to avoid conflicts with other modules.
   */
  void initModuleOnLocal(const ObjectFileLocationFilePath& location, const std::string& config,
                         const std::string& uuid);

  /**
   * Initialize the module on the remote http uri.
   * @param bytes the bytes of the object file.
   * @param config the configuration for the module.
   * @param uuid a random uuid to avoid conflicts with other modules.
   */
  void initModuleOnInlineBytes(const ObjectFileLocationInlineBytes& bytes,
                               const std::string& config, const std::string& uuid);

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

  // The in-memory context for the module.
  void* module_ctx_ = nullptr;

private:
  const std::string name_;
  // The raw dlopen handle.
  void* handle_ = nullptr;
  // The path to clean up.
  std::string file_path_for_cleanup_;
};

using DynamicModuleSharedPtr = std::shared_ptr<DynamicModule>;

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
