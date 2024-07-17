
#include <exception>
#include <vector>
#include <format>

#include "source/common/common/assert.h"

#include "x/dynamic_module.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

DynamicModuleSharedPtr loadTestDynamicModule(const std::string& file_path,
                                             const std::string& uuid = "",
                                             const std::string& config = "",
                                             const std::string& name = "") {

  constexpr auto path_fmt = "./test/test_programs/lib{}.so";
  const auto path = fmt::format(path_fmt, file_path);
  return std::make_shared<DynamicModule>(name, ObjectFileLocationFilePath{path}, config, uuid);
}

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
