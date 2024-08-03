#include <memory>
#include <string>
#include <string_view>

#include "source/extensions/dynamic_modules/dynamic_modules.h"
#include "source/http_dynamic_module.h"
#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "source/config.pb.h"
#include "source/config.pb.validate.h"
#include "source/filter.h"

namespace Envoy {
namespace Server {
namespace Configuration {

using DynamicModuleConfig =
    envoy::extensions::filters::http::dynamic_modules::v3::DynamicModuleConfig;

class DynamicModuleFactory : public NamedHttpFilterConfigFactory {
public:
  absl::StatusOr<Http::FilterFactoryCb>
  createFilterFactoryFromProto(const Protobuf::Message& proto_config, const std::string&,
                               FactoryContext& context) override {

    return createFactory(Envoy::MessageUtil::downcastAndValidate<const DynamicModuleConfig&>(
                             proto_config, context.messageValidationVisitor()),
                         context);
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new DynamicModuleConfig()};
  }

  std::string name() const override { return "envoy.http.dynamic_modules"; }

private:
  Http::FilterFactoryCb createFactory(const DynamicModuleConfig& proto_config, FactoryContext&) {
    const auto dynamic_module = Extensions::DynamicModules::newDynamicModule(
        proto_config.file_path(), proto_config.do_not_dlclose());
    if (!dynamic_module.ok()) {
      throw EnvoyException("Failed to load dynamic module: " +
                           std::string(dynamic_module.status().message()));
    }
    auto http_dynamic_module =
        std::make_shared<Envoy::Extensions::DynamicModules::Http::HttpDynamicModule>(
            proto_config.name(), proto_config.filter_config(), dynamic_module.value());

    return [http_dynamic_module](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      auto filter = std::make_shared<Envoy::Extensions::DynamicModules::Http::HttpFilter>(
          http_dynamic_module);
      callbacks.addStreamDecoderFilter(filter);
      callbacks.addStreamEncoderFilter(filter);
    };
  }
};

/**
 * Static registration for this sample filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<DynamicModuleFactory, NamedHttpFilterConfigFactory> register_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy