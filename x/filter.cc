#include <string>

#include "filter.h"

#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

#define STATIC_CAST_AS_VOID(x) static_cast<void*>(x)
#define THIS_AS_VOID STATIC_CAST_AS_VOID(this)

HttpFilter::HttpFilter(DynamicModuleSharedPtr dynamic_module) : dynamic_module_(dynamic_module) {}

HttpFilter::~HttpFilter() = default;

void HttpFilter::onDestroy() {
  if (stream_context_) {
    dynamic_module_->envoyModuleHttpOnDestroy()(THIS_AS_VOID, stream_context_);
  }
};

FilterHeadersStatus HttpFilter::decodeHeaders(RequestHeaderMap& headers, bool end_of_stream) {
  if (!stream_context_) {
    stream_context_ = dynamic_module_->envoyModuleHttpContextInit()();
    if (!stream_context_) {
      return FilterHeadersStatus::StopIteration;
    }
  }
  ASSERT(stream_context_);
  const ABI::OnRequestHeadersStatus result = dynamic_module_->envoyModuleHttpOnRequestHeaders()(
      THIS_AS_VOID, stream_context_, STATIC_CAST_AS_VOID(&headers), end_of_stream);
  return static_cast<FilterHeadersStatus>(result);
};

FilterDataStatus HttpFilter::decodeData(Buffer::Instance& buffer, bool end_of_stream) {
  ASSERT(stream_context_);
  const ABI::OnRequestBodyStatus result = dynamic_module_->envoyModuleHttpOnRequestBody()(
      THIS_AS_VOID, stream_context_, STATIC_CAST_AS_VOID(&buffer), end_of_stream);
  return static_cast<FilterDataStatus>(result);
};

FilterHeadersStatus HttpFilter::encodeHeaders(ResponseHeaderMap& headers, bool end_of_stream) {
  ASSERT(stream_context_);
  const ABI::OnResponseHeadersStatus result = dynamic_module_->envoyModuleHttpOnResponseHeaders()(
      THIS_AS_VOID, stream_context_, STATIC_CAST_AS_VOID(&headers), end_of_stream);
  return static_cast<FilterHeadersStatus>(result);
};

FilterDataStatus HttpFilter::encodeData(Buffer::Instance& buffer, bool end_of_stream) {
  ASSERT(stream_context_);
  const ABI::OnResponseBodyStatus result = dynamic_module_->envoyModuleHttpOnResponseBody()(
      THIS_AS_VOID, stream_context_, STATIC_CAST_AS_VOID(&buffer), end_of_stream);
  return static_cast<FilterDataStatus>(result);
};

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy