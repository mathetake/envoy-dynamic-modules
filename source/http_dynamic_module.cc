#include <filesystem>
#include <string>

#include "http_dynamic_module.h"
#include "source/http_dynamic_module.h"

#include <dlfcn.h>

#include "source/common/common/assert.h"
#include "envoy/common/exception.h"

namespace Envoy {
namespace Extensions {
namespace DynamicModules {
namespace Http {

HttpDynamicModule::~HttpDynamicModule() {
  ENVOY_LOG_MISC(info, "Destroying module: {}", name_);
  envoy_dynamic_module_event_http_filter_destroy_(http_filter_);
}

#define RESOLVE_SYMBOL_OR_THROW(symbol_type)                                                       \
  do {                                                                                             \
    symbol_type##_ = dynamic_module_->getFunctionPointer<symbol_type>(#symbol_type);               \
    if (symbol_type##_ == nullptr) {                                                               \
      throw EnvoyException(                                                                        \
          fmt::format("cannot resolve symbol: {} error: {}", #symbol_type, dlerror()));            \
    }                                                                                              \
  } while (0)

void HttpDynamicModule::initHttpFilter(const std::string_view config) {
  RESOLVE_SYMBOL_OR_THROW(envoy_dynamic_module_event_http_filter_init);
  ENVOY_LOG_MISC(info, "[{}] -> envoy_dynamic_module_event_http_filter_init ({}, {})", name_,
                 const_cast<char*>(config.data()), config.size());
  http_filter_ =
      envoy_dynamic_module_event_http_filter_init_(const_cast<char*>(config.data()), config.size());
  if (http_filter_ == nullptr) {
    throw EnvoyException(fmt::format("http filter init in {} failed", name_));
  }
  ENVOY_LOG_MISC(info, "[{}] <- envoy_dynamic_module_event_http_filter_init: {}", name_,
                 http_filter_);
  RESOLVE_SYMBOL_OR_THROW(envoy_dynamic_module_event_http_filter_destroy);
  RESOLVE_SYMBOL_OR_THROW(envoy_dynamic_module_event_http_filter_instance_init);
  RESOLVE_SYMBOL_OR_THROW(envoy_dynamic_module_event_http_filter_instance_request_headers);
  RESOLVE_SYMBOL_OR_THROW(envoy_dynamic_module_event_http_filter_instance_request_body);
  RESOLVE_SYMBOL_OR_THROW(envoy_dynamic_module_event_http_filter_instance_response_headers);
  RESOLVE_SYMBOL_OR_THROW(envoy_dynamic_module_event_http_filter_instance_response_body);
  RESOLVE_SYMBOL_OR_THROW(envoy_dynamic_module_event_http_filter_instance_destroy);
}

#undef RESOLVE_SYMBOL_OR_THROW

} // namespace Http
} // namespace DynamicModules
} // namespace Extensions
} // namespace Envoy
