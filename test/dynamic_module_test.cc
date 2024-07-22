#include "gtest/gtest.h"
#include <cstddef>
#include <exception>
#include <vector>

#include "x/dynamic_module.h"

#include "test/test_util.h"
#include "test/test_common/utility.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

TEST(TestDynamicModule, InvalidPath) {
  EXPECT_THROW_WITH_REGEX(loadTestDynamicModule("non_exist"), std::exception,
                          "filesystem error: in copy: No such file or directory");
}

TEST(TestDynamicModule, InitNonExist) {
  EXPECT_THROW_WITH_REGEX(
      loadTestDynamicModule("no_init", "InitNonExist", "config"), EnvoyException,
      "cannot resolve symbol: __envoy_dynamic_module_v1_event_http_filter_init");
}

TEST(TestDynamicModule, InitFail) {
  EXPECT_THROW_WITH_REGEX(loadTestDynamicModule("init_fail", "InitFail", "", "fooooooooooo"),
                          EnvoyException, "init function in fooooooooooo failed");
}

TEST(TestDynamicModule, ConstructorHappyPath) {
  // Ensures that the module can be loaded multiple times independently.
  std::vector<DynamicModuleSharedPtr> modules;
  for (int i = 0; i < 10; i++) {
    std::string config = "config";
    DynamicModuleSharedPtr module =
        loadTestDynamicModule("init", "ConstructorHappyPath" + std::to_string(i), config);

    // We intentionally set the config to "111111" in the init function, so check it.
    EXPECT_EQ(config, "111111");
    modules.push_back(module);
  }

  // Check handles are different.
  for (int i = 0; i < 9; i++) {
    EXPECT_NE(modules[i]->handleForTesting(), modules[i + 1]->handleForTesting());
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
    module1 = std::make_shared<DynamicModule>("", ObjectFileLocationFilePath{path.string()}, config,
                                              "TestDynamicModule_SameNameDifferentFile_old");
  }

  // Delete the file.
  std::filesystem::remove(path);

  DynamicModuleSharedPtr module2 = nullptr;
  {
    const std::string file_path = "./test/test_programs/libstream_init.so";
    std::filesystem::copy(file_path, path, std::filesystem::copy_options::recursive);
    module2 = std::make_shared<DynamicModule>("", ObjectFileLocationFilePath{path.string()}, config,
                                              "TestDynamicModule_SameNameDifferentFile_new");
  }

  // Even after the file is deleted, the module should still be able to be loaded. Just make sure
  // that functions can be called.
  module1->__envoy_dynamic_module_v1_event_http_filter_instance_init_(nullptr, nullptr);
  module2->__envoy_dynamic_module_v1_event_http_filter_instance_init_(nullptr, nullptr);

  // Check handles are different because the uuid is different even when the file name is the same.
  EXPECT_NE(module1->handleForTesting(), module2->handleForTesting());

  std::filesystem::remove(path);
}

TEST(TestDynamicModule, InlineBytes_OK) {
  // Load the test program from test_programs/libinit.so.
  const std::string config = "config";
  const std::string uuid = "InlineBytes_OK";
  constexpr auto path_fmt = "./test/test_programs/libinit.so";

  // Read all bytes from the file.
  std::ifstream file(path_fmt, std::ios::in | std::ios::binary | std::ios::ate);
  ASSERT(file.is_open());
  const auto size = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<char> bytes(size);
  file.read(bytes.data(), size);
  file.close();

  // Cast the bytes to a string.
  const std::string bytes_str(bytes.begin(), bytes.end());

  // Load the module from the bytes.
  DynamicModuleSharedPtr module =
      std::make_shared<DynamicModule>("", ObjectFileLocationInlineBytes{bytes_str}, config, uuid);

  // We intentionally set the config to "111111" in the init function, so check it.
  EXPECT_EQ(config, "111111");
}

TEST(TestDynamicModule, InlineBytes_Fail) {
  const std::string bytes = "inline_bytes";
  const std::string config = "config";
  const std::string uuid = "InlineBytes_Fail";
  EXPECT_THROW_WITH_REGEX(
      DynamicModule module("", ObjectFileLocationInlineBytes{bytes}, config, uuid), EnvoyException,
      "cannot load .* error: .*");
}

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
