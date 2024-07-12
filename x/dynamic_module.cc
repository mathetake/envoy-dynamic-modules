#include <filesystem>
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
  std::filesystem::remove(copied_file_path_);
}

DynamicModule::DynamicModule(const std::string& file_path, const std::string& config,
                             const std::string&& uuid) {
  // dlopen does only return a new handle if the file is not already loaded. If the inode
  // of the file is the same as a file that is already loaded, dlopen will return the same one.
  //
  // To reload a module, we need to create a hard copy of the file and load that.
  const std::filesystem::path file_path_absolute = std::filesystem::absolute(file_path);
  copied_file_path_ = file_path_absolute.string() + "." + uuid;

  std::filesystem::copy(file_path_absolute, copied_file_path_,
                        std::filesystem::copy_options::recursive);

  // Load the module with RTLD_LOCAL to avoid symbol conflicts with other modules.
  handler_ = dlopen(copied_file_path_.c_str(), RTLD_LOCAL | RTLD_LAZY);
  if (!handler_) {
    throw EnvoyException(fmt::format("cannot load : {} error: {}", copied_file_path_, dlerror()));
  }

  // Initialize the module.
  initModule(config);
}

void DynamicModule::initModule(const std::string& config) {
  const Symbols::EnvoyModuleInit init =
      Symbols::resolveSymbol<Symbols::EnvoyModuleInit, Symbols::__envoy_module_init>(handler_);

  if (!init) {
    throw EnvoyException(fmt::format("cannot find init function in {}", copied_file_path_));
  }
  const int result = init(config.data());
  if (result != 0) {
    throw EnvoyException(
        fmt::format("init function in {} failed with result {}", copied_file_path_, result));
  }

  envoy_module_http_stream_context_init_ =
      Symbols::resolveSymbol<Symbols::EnvoyModuleHttpStreamContextInit,
                             Symbols::__envoy_module_http_stream_context_init>(handler_);
  if (!envoy_module_http_stream_context_init_) {
    throw EnvoyException(
        fmt::format("cannot find http stream init function in {}", copied_file_path_));
  }
}

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
