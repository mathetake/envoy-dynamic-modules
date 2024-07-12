#include <string>

#include "filter.h"

#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

#define THIS_AS_VOID static_cast<void*>(this)

HttpFilter::HttpFilter(DynamicModuleSharedPtr dynamic_module)
    : dynamic_module_(dynamic_module), stream_context_(nullptr) {}

HttpFilter::~HttpFilter() = default;

void HttpFilter::onDestroy() {
  if (stream_context_) {
    dynamic_module_->envoyModuleHttpOnDestroy()(THIS_AS_VOID, stream_context_);
  }
};

FilterHeadersStatus HttpFilter::decodeHeaders(RequestHeaderMap&, bool) {
  if (!stream_context_) {
    stream_context_ = dynamic_module_->envoyModuleHttpContextInit()();
    if (!stream_context_) {
      return FilterHeadersStatus::StopIteration;
    }
  }
  ASSERT(stream_context_);
  const int result =
      dynamic_module_->envoyModuleHttpOnRequestHeaders()(THIS_AS_VOID, stream_context_);
  return static_cast<FilterHeadersStatus>(result);
};

FilterDataStatus HttpFilter::decodeData(Buffer::Instance&, bool) {
  ASSERT(stream_context_);
  const int result = dynamic_module_->envoyModuleHttpOnRequestBody()(THIS_AS_VOID, stream_context_);
  return static_cast<FilterDataStatus>(result);
};

FilterHeadersStatus HttpFilter::encodeHeaders(ResponseHeaderMap&, bool) {
  ASSERT(stream_context_);
  const int result =
      dynamic_module_->envoyModuleHttpOnResponseHeaders()(THIS_AS_VOID, stream_context_);
  return static_cast<FilterHeadersStatus>(result);
};

FilterDataStatus HttpFilter::encodeData(Buffer::Instance&, bool) {
  ASSERT(stream_context_);
  const int result =
      dynamic_module_->envoyModuleHttpOnResponseBody()(THIS_AS_VOID, stream_context_);
  return static_cast<FilterDataStatus>(result);
};

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy