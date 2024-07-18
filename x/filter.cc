#include <string>

#include "filter.h"

#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

#define STATIC_CAST_AS_VOID(x) static_cast<void*>(x)
#define THIS_AS_VOID STATIC_CAST_AS_VOID(this)

HttpFilter::HttpFilter(DynamicModuleSharedPtr dynamic_module) : dynamic_module_(dynamic_module) {}

HttpFilter::~HttpFilter() { this->destoryStreamContext(); }

void HttpFilter::onDestroy() { this->destoryStreamContext(); };

void HttpFilter::destoryStreamContext() {
  ASSERT(dynamic_module_);
  if (stream_context_) {
    dynamic_module_->__envoy_dynamic_module_v1_event_http_destroy_(THIS_AS_VOID, stream_context_);
    stream_context_ = nullptr;
  }
}

FilterHeadersStatus HttpFilter::decodeHeaders(RequestHeaderMap& headers, bool end_of_stream) {
  ASSERT(dynamic_module_);
  if (!stream_context_) {
    this->ensureStreamContext();
    if (!stream_context_) {
      return FilterHeadersStatus::StopIteration;
    }
  }
  ASSERT(stream_context_);
  const __envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatus result =
      dynamic_module_->__envoy_dynamic_module_v1_event_http_request_headers_(
          THIS_AS_VOID, stream_context_, STATIC_CAST_AS_VOID(&headers), end_of_stream);
  return static_cast<FilterHeadersStatus>(result);
};

FilterDataStatus HttpFilter::decodeData(Buffer::Instance& buffer, bool end_of_stream) {
  ASSERT(dynamic_module_);
  ASSERT(stream_context_);
  const __envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus result =
      dynamic_module_->__envoy_dynamic_module_v1_event_http_request_body_(
          THIS_AS_VOID, stream_context_, STATIC_CAST_AS_VOID(&buffer), end_of_stream);
  return static_cast<FilterDataStatus>(result);
};

FilterHeadersStatus HttpFilter::encodeHeaders(ResponseHeaderMap& headers, bool end_of_stream) {
  ASSERT(dynamic_module_);
  ASSERT(stream_context_);
  const __envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatus result =
      dynamic_module_->__envoy_dynamic_module_v1_event_http_response_headers_(
          THIS_AS_VOID, stream_context_, STATIC_CAST_AS_VOID(&headers), end_of_stream);
  return static_cast<FilterHeadersStatus>(result);
};

FilterDataStatus HttpFilter::encodeData(Buffer::Instance& buffer, bool end_of_stream) {
  ASSERT(dynamic_module_);
  ASSERT(stream_context_);
  const __envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus result =
      dynamic_module_->__envoy_dynamic_module_v1_event_http_response_body_(
          THIS_AS_VOID, stream_context_, STATIC_CAST_AS_VOID(&buffer), end_of_stream);

  return static_cast<FilterDataStatus>(result);
};

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy