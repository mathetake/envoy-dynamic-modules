#include <memory>
#include <string>
#include <string_view>

#include "dynamic_module.h"
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

    Http::DynamicModule::ObjectFileLocation location;
    if (proto_config.has_file_path()) {
      location = Http::DynamicModule::ObjectFileLocationFilePath{
          std::string_view(proto_config.file_path())};
    } else {
      location = Http::DynamicModule::ObjectFileLocationInlineBytes{
          std::string_view(proto_config.inline_bytes())};
    }

    auto config = std::make_shared<Http::DynamicModule::DynamicModule>(
        proto_config.name(), location, proto_config.module_config(),
        context.serverFactoryContext().api().randomGenerator().uuid());

    return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      auto filter = std::make_shared<Http::DynamicModule::HttpFilter>(config);
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