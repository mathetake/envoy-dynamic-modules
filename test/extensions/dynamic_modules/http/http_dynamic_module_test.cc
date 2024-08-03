#include "gtest/gtest.h"
#include <cstddef>
#include <exception>
#include <vector>

#include "source/extensions/dynamic_modules/http/http_dynamic_module.h"

#include "test/extensions/dynamic_modules/http/test_util.h"
#include "test/test_common/utility.h"

namespace Envoy {
namespace Extensions {
namespace DynamicModules {
namespace Http {

TEST(TestDynamicModule, InvalidPath) {
  EXPECT_THROW_WITH_REGEX(loadTestDynamicModule("non_exist"), EnvoyException,
                          "Failed to load dynamic module:");
}

TEST(TestDynamicModule, InitNonExist) {
  EXPECT_THROW_WITH_REGEX(loadTestDynamicModule("no_init", "config"), EnvoyException,
                          "Failed to resolve envoy_dynamic_module_event_program_init");
}

TEST(TestDynamicModule, ProgramIwnitFail) {
  EXPECT_THROW_WITH_REGEX(loadTestDynamicModule("program_init_fail", "", "fooooooooooo"),
                          EnvoyException, "envoy_dynamic_module_event_program_init failed:");
}

TEST(TestDynamicModule, HttpFilterInitFail) {
  EXPECT_THROW_WITH_REGEX(loadTestDynamicModule("http_filter_init_fail", "", "baaaaaaaaaa"),
                          EnvoyException, "http filter init in baaaaaaaaaa failed");
}

TEST(TestDynamicModule, DoNotClose) {
  size_t* in_module_ptr = nullptr;
  {
    HttpDynamicModuleSharedPtr module = loadTestDynamicModule("do_not_close", "", "", true);
    in_module_ptr = (size_t*)module->envoy_dynamic_module_event_http_filter_init_(nullptr, 0);
    module.reset();
  }

  // The module is not closed, so the pointer is still valid.
  EXPECT_EQ(999999, *in_module_ptr);
}

TEST(TestDynamicModule, ConstructorHappyPath) {
  // Ensures that the module can be loaded multiple times independently but only program init is
  // called once.
  std::vector<HttpDynamicModuleSharedPtr> modules;
  for (int i = 0; i < 10; i++) {
    std::string config = "config";
    HttpDynamicModuleSharedPtr module = loadTestDynamicModule("init", config, std::to_string(i));

    // We intentionally set the config to "111111" in the init function, so check it.
    EXPECT_EQ(config, "111111");
    modules.push_back(module);
  }

  // Check handles are the same for all modules.
  for (int i = 0; i < 9; i++) {
    EXPECT_EQ(modules[i]->dynamic_module_->handleForTesting(),
              modules[i + 1]->dynamic_module_->handleForTesting());
  }

  auto first = modules[0];
  // Close the non-first module.
  for (int i = 1; i < 10; i++) {
    modules[i].reset();
  }

  // But still the first module is alive.
  first->envoy_dynamic_module_event_http_filter_instance_init_(nullptr, 0);
}

} // namespace Http
} // namespace DynamicModules
} // namespace Extensions
} // namespace Envoy
