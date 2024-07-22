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

void HttpFilter::ensureStreamContext() {
  ENVOY_LOG_MISC(info,
                 "[{}] -> __envoy_dynamic_module_v1_event_http_filter_instance_init_ ({}, {})",
                 dynamic_module_->name_, THIS_AS_VOID, dynamic_module_->module_ctx_);
  stream_context_ = dynamic_module_->__envoy_dynamic_module_v1_event_http_filter_instance_init_(
      THIS_AS_VOID, dynamic_module_->module_ctx_);
  ENVOY_LOG_MISC(info, "[{}] <- __envoy_dynamic_module_v1_event_http_filter_instance_init_: {}",
                 dynamic_module_->name_, stream_context_);
}

void HttpFilter::onDestroy() { this->destoryStreamContext(); };

void HttpFilter::destoryStreamContext() {
  this->encoder_callbacks_ = nullptr;
  this->decoder_callbacks_ = nullptr;
  ASSERT(dynamic_module_);
  if (stream_context_) {
    ENVOY_LOG_MISC(info,
                   "[{}] -> __envoy_dynamic_module_v1_event_http_filter_instance_destroy_ ({})",
                   dynamic_module_->name_, stream_context_);
    dynamic_module_->__envoy_dynamic_module_v1_event_http_filter_instance_destroy_(stream_context_);
    ENVOY_LOG_MISC(info, "[{}] <- __envoy_dynamic_module_v1_event_http_filter_instance_destroy_",
                   dynamic_module_->name_);
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
  ENVOY_LOG_MISC(
      info,
      "[{}] -> __envoy_dynamic_module_v1_event_http_filter_instance_request_headers_ ({}, {}, {})",
      dynamic_module_->name_, stream_context_, STATIC_CAST_AS_VOID(&headers), end_of_stream);
  const __envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatus result =
      dynamic_module_->__envoy_dynamic_module_v1_event_http_filter_instance_request_headers_(
          stream_context_, STATIC_CAST_AS_VOID(&headers), end_of_stream);
  ENVOY_LOG_MISC(
      info, "[{}] <- __envoy_dynamic_module_v1_event_http_filter_instance_request_headers_: {}",
      dynamic_module_->name_, result);
  this->in_continue_ =
      result == __envoy_dynamic_module_v1_type_EventHttpRequestHeadersStatusContinue;
  return static_cast<FilterHeadersStatus>(result);
};

FilterDataStatus HttpFilter::decodeData(Buffer::Instance& buffer, bool end_of_stream) {
  ASSERT(dynamic_module_);
  ASSERT(stream_context_);
  ENVOY_LOG_MISC(
      info,
      "[{}] -> __envoy_dynamic_module_v1_event_http_filter_instance_request_body_ ({}, {}, {})",
      dynamic_module_->name_, stream_context_, STATIC_CAST_AS_VOID(&buffer), end_of_stream);
  const __envoy_dynamic_module_v1_type_EventHttpRequestBodyStatus result =
      dynamic_module_->__envoy_dynamic_module_v1_event_http_filter_instance_request_body_(
          stream_context_, STATIC_CAST_AS_VOID(&buffer), end_of_stream);
  ENVOY_LOG_MISC(info,
                 "[{}] <- __envoy_dynamic_module_v1_event_http_filter_instance_request_body_: {}",
                 dynamic_module_->name_, result);
  this->in_continue_ = result == __envoy_dynamic_module_v1_type_EventHttpRequestBodyStatusContinue;
  return static_cast<FilterDataStatus>(result);
};

FilterHeadersStatus HttpFilter::encodeHeaders(ResponseHeaderMap& headers, bool end_of_stream) {
  ASSERT(dynamic_module_);
  ASSERT(stream_context_);
  ENVOY_LOG_MISC(
      info,
      "[{}] -> __envoy_dynamic_module_v1_event_http_filter_instance_response_headers_ ({}, {}, {})",
      dynamic_module_->name_, stream_context_, STATIC_CAST_AS_VOID(&headers), end_of_stream);
  const __envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatus result =
      dynamic_module_->__envoy_dynamic_module_v1_event_http_filter_instance_response_headers_(
          stream_context_, STATIC_CAST_AS_VOID(&headers), end_of_stream);
  ENVOY_LOG_MISC(
      info, "[{}] <- __envoy_dynamic_module_v1_event_http_filter_instance_response_headers_: {}",
      dynamic_module_->name_, result);
  this->in_continue_ =
      result == __envoy_dynamic_module_v1_type_EventHttpResponseHeadersStatusContinue;
  return static_cast<FilterHeadersStatus>(result);
};

FilterDataStatus HttpFilter::encodeData(Buffer::Instance& buffer, bool end_of_stream) {
  ASSERT(dynamic_module_);
  ASSERT(stream_context_);
  ENVOY_LOG_MISC(
      info,
      "[{}] -> __envoy_dynamic_module_v1_event_http_filter_instance_response_body_ ({}, {}, {})",
      dynamic_module_->name_, stream_context_, STATIC_CAST_AS_VOID(&buffer), end_of_stream);
  const __envoy_dynamic_module_v1_type_EventHttpResponseBodyStatus result =
      dynamic_module_->__envoy_dynamic_module_v1_event_http_filter_instance_response_body_(
          stream_context_, STATIC_CAST_AS_VOID(&buffer), end_of_stream);
  ENVOY_LOG_MISC(info,
                 "[{}] <- __envoy_dynamic_module_v1_event_http_filter_instance_response_body_: {}",
                 dynamic_module_->name_, result);
  this->in_continue_ = result == __envoy_dynamic_module_v1_type_EventHttpResponseBodyStatusContinue;
  return static_cast<FilterDataStatus>(result);
};

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy