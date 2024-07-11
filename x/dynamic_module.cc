#include <string>

#include "x/dynamic_module.h"

#include <dlfcn.h>

#include "source/common/common/assert.h"
#include "envoy/common/exception.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

DynamicModule::~DynamicModule() {
  ASSERT(handler_ != nullptr);
  dlclose(handler_);
  unlink(symlink_path_.c_str());
}

DynamicModule::DynamicModule(const std::string& file_path, const std::string& config,
                             const std::string&& uuid) {

  // dlopen does not take care of the content of the file. It just checks the file name,
  // and if the name is the same as the one of a previously loaded library, it returns the
  // same handle.
  //
  // The problem can be avoided by creating a symlink to the file before loading it.
  // First create a symlink to the file to avoid the reuse of the same file.
  //

  auto file_path_absolute = std::filesystem::absolute(file_path);
  symlink_path_ = file_path_absolute.string() + "." + uuid;

  // Create the symlink.
  if (symlink(file_path_absolute.c_str(), symlink_path_.c_str()) != 0) {
    throw EnvoyException(
        fmt::format("cannot create symlink: {} error: {}", symlink_path_, strerror(errno)));
  }

  // Load the module with RTLD_LOCAL to avoid symbol conflicts with other modules.
  handler_ = dlopen(symlink_path_.c_str(), RTLD_LOCAL | RTLD_LAZY);
  if (!handler_) {
    unlink(symlink_path_.c_str());
    throw EnvoyException(fmt::format("cannot load : {} error: {}", symlink_path_, dlerror()));
  }

  // Initialize the module.
  initModule(config);
}

void DynamicModule::initModule(const std::string& config) {
  auto init = reinterpret_cast<Symbols::EnvoyInit>(dlsym(handler_, Symbols::INIT_FUNCTION_NAME));

  if (!init) {
    throw EnvoyException(fmt::format("cannot find init function in {}", symlink_path_));
  }
  const int result = init(config.data());
  if (result != 0) {
    throw EnvoyException(
        fmt::format("init function in {} failed with result {}", symlink_path_, result));
  }
}

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
