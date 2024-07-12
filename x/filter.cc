#include <string>

#include "filter.h"

#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

HttpFilter::HttpFilter(DynamicModuleSharedPtr dynamic_module)
    : dynamic_module_(dynamic_module), stream_context_(nullptr) {}

HttpFilter::~HttpFilter() = default;

void HttpFilter::onDestroy(){};

FilterHeadersStatus HttpFilter::decodeHeaders(RequestHeaderMap&, bool) {
  if (!stream_context_) {
    stream_context_ = dynamic_module_->envoyModuleHttpStreamContextInit()();
    if (!stream_context_) {
      return FilterHeadersStatus::StopIteration;
    }
  }
  ASSERT(stream_context_);
  return FilterHeadersStatus::Continue;
};

FilterDataStatus HttpFilter::decodeData(Buffer::Instance&, bool) {
  ASSERT(stream_context_);
  return FilterDataStatus::Continue;
};

FilterHeadersStatus HttpFilter::encodeHeaders(ResponseHeaderMap&, bool) {
  ASSERT(stream_context_);
  return FilterHeadersStatus::Continue;
};

FilterDataStatus HttpFilter::encodeData(Buffer::Instance&, bool) {
  ASSERT(stream_context_);
  return FilterDataStatus::Continue;
};

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy