
#pragma once

#include <string>

#include "envoy/server/filter_config.h"

#include "x/config.pb.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

namespace Symbols {
using EnvoyInit = int (*)(const char*);
constexpr char INIT_FUNCTION_NAME[] = "__envoy_init";

} // namespace Symbols

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

private:
  void* handler_ = nullptr;
  std::string symlink_path_;
};

using DynamicModuleSharedPtr = std::shared_ptr<DynamicModule>;

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
