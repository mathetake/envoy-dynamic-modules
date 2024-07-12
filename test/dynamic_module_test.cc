#include "gtest/gtest.h"
#include <exception>
#include <vector>

#include "x/dynamic_module.h"

#include "test/test_common/utility.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

TEST(TestDynamicModule, InvalidPath) {
  EXPECT_THROW_WITH_REGEX(DynamicModule("./non_exist.so", "config", "aaaa"), std::exception,
                          "filesystem error: in copy: No such file or directory");
}

TEST(TestDynamicModule, InitNonExist) {
  EXPECT_THROW_WITH_REGEX(
      DynamicModule("./test/test_programs/libno_init.so", "config", "InitNonExist"), EnvoyException,
      "cannot find init function in");
}

TEST(TestDynamicModule, InitFail) {
  EXPECT_THROW_WITH_REGEX(
      DynamicModule("./test/test_programs/libinit_fail.so", "config", "InitFail"), EnvoyException,
      "init function in .* failed with result 12345");
}

TEST(TestDynamicModule, ConstructorHappyPath) {
  // Ensures that the module can be loaded multiple times independently.
  std::vector<DynamicModuleSharedPtr> modules;
  for (int i = 0; i < 10; i++) {
    std::string config = "config";
    DynamicModuleSharedPtr module = std::make_shared<DynamicModule>(
        "./test/test_programs/libinit.so", config, "ConstructorHappyPath" + std::to_string(i));
    // We intentionally set the config to "111111" in the init function, so check it.
    EXPECT_EQ(config, "111111");
    modules.push_back(module);
  }

  // Check handlers are different.
  for (int i = 0; i < 9; i++) {
    EXPECT_NE(modules[i]->handlerForTesting(), modules[i + 1]->handlerForTesting());
  }
}

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
