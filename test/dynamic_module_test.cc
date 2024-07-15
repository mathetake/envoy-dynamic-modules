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
      "cannot resolve symbol: __envoy_dynamic_module_v1_init");
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

TEST(TestDynamicModule, SameNameDifferentFile) {
  const std::string config = "config";
  const std::filesystem::path tmpdir = std::filesystem::temp_directory_path();
  const auto path = tmpdir / "libtmptmp_TestDynamicModule_SameNameDifferentFile.so";

  // Clean up the file if it exists.
  std::filesystem::remove(path);

  DynamicModuleSharedPtr module1 = nullptr;
  {
    const std::string file_path = "./test/test_programs/libinit.so";
    std::filesystem::copy(file_path, path, std::filesystem::copy_options::recursive);
    module1 = std::make_shared<DynamicModule>(path.string(), config,
                                              "TestDynamicModule_SameNameDifferentFile_old");
  }

  // Delete the file.
  std::filesystem::remove(path);

  DynamicModuleSharedPtr module2 = nullptr;
  {
    const std::string file_path = "./test/test_programs/libstream_init.so";
    std::filesystem::copy(file_path, path, std::filesystem::copy_options::recursive);
    module2 = std::make_shared<DynamicModule>(path.string(), config,
                                              "TestDynamicModule_SameNameDifferentFile_new");
  }

  // Even after the file is deleted, the module should still be able to be loaded. Just make sure
  // that functions can be called.
  module1->envoyModuleHttpContextInit()();
  module2->envoyModuleHttpContextInit()();

  // Check handlers are different because the uuid is different even when the file name is the same.
  EXPECT_NE(module1->handlerForTesting(), module2->handlerForTesting());

  std::filesystem::remove(path);
}

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
