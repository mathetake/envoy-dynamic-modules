
#include <exception>
#include <vector>
#include <format>

#include "source/common/common/assert.h"

#include "source/extensions/dynamic_modules/http/http_dynamic_module.h"

namespace Envoy {
namespace Extensions {
namespace DynamicModules {
namespace Http {

HttpDynamicModuleSharedPtr loadTestDynamicModule(const std::string& file_path,
                                                 const std::string& config = "",
                                                 const std::string& name = "",
                                                 const bool do_not_dlclose = false) {
  constexpr auto path_fmt = "./test/test_programs/lib{}.so";
  const auto path = fmt::format(path_fmt, file_path);

  const auto dynamic_module = Extensions::DynamicModules::newDynamicModule(path, do_not_dlclose);
  if (!dynamic_module.ok()) {
    throw EnvoyException(std::string(dynamic_module.status().message()));
  }

  auto http_dynamic_module =
      std::make_shared<Envoy::Extensions::DynamicModules::Http::HttpDynamicModule>(
          name, config, dynamic_module.value());

  return http_dynamic_module;
}

} // namespace Http
} // namespace DynamicModules
} // namespace Extensions
} // namespace Envoy
