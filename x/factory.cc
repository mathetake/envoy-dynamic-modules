#include <memory>
#include <string>

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "x/config.pb.h"
#include "x/config.pb.validate.h"
#include "x/filter.h"

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
  Http::FilterFactoryCb createFactory(const DynamicModuleConfig& proto_config,
                                      FactoryContext& context) {
    Http::DynamicModule::DynamicModuleSharedPtr config =
        std::make_shared<Http::DynamicModule::DynamicModule>(Http::DynamicModule::DynamicModule(
            proto_config.file_path(), proto_config.module_config().value(),
            std::move(context.serverFactoryContext().api().randomGenerator().uuid())));

    return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      auto filter = std::make_shared<Http::DynamicModule::HttpFilter>(config);
      callbacks.addStreamDecoderFilter(filter);
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