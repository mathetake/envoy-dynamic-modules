#include <string>

#include "filter.h"

#include "envoy/server/filter_config.h"
#include "source/http_dynamic_module.h"

namespace Envoy {
namespace Extensions {
namespace DynamicModules {
namespace Http {

#define STATIC_CAST_AS_VOID(x) static_cast<void*>(x)
#define THIS_AS_VOID STATIC_CAST_AS_VOID(this)

HttpFilter::HttpFilter(HttpDynamicModuleSharedPtr dynamic_module)
    : dynamic_module_(dynamic_module) {}

HttpFilter::~HttpFilter() { this->destoryHttpFilterInstance(); }

void HttpFilter::ensureHttpFilterInstance() {
  ENVOY_LOG_MISC(info, "[{}] -> envoy_dynamic_module_event_http_filter_instance_init_ ({}, {})",
                 dynamic_module_->name_, THIS_AS_VOID, dynamic_module_->http_filter_);
  http_filter_instance_ = dynamic_module_->envoy_dynamic_module_event_http_filter_instance_init_(
      THIS_AS_VOID, dynamic_module_->http_filter_);
  ENVOY_LOG_MISC(info, "[{}] <- envoy_dynamic_module_event_http_filter_instance_init_: {}",
                 dynamic_module_->name_, http_filter_instance_);
}

void HttpFilter::onDestroy() { this->destoryHttpFilterInstance(); };

void HttpFilter::destoryHttpFilterInstance() {
  this->encoder_callbacks_ = nullptr;
  this->decoder_callbacks_ = nullptr;
  ASSERT(dynamic_module_);
  if (http_filter_instance_) {
    ENVOY_LOG_MISC(info, "[{}] -> envoy_dynamic_module_event_http_filter_instance_destroy_ ({})",
                   dynamic_module_->name_, http_filter_instance_);
    dynamic_module_->envoy_dynamic_module_event_http_filter_instance_destroy_(
        http_filter_instance_);
    ENVOY_LOG_MISC(info, "[{}] <- envoy_dynamic_module_event_http_filter_instance_destroy_",
                   dynamic_module_->name_);
    http_filter_instance_ = nullptr;
  }
}

FilterHeadersStatus HttpFilter::decodeHeaders(RequestHeaderMap& headers, bool end_of_stream) {
  ASSERT(dynamic_module_);
  if (!http_filter_instance_) {
    this->ensureHttpFilterInstance();
    if (!http_filter_instance_) {
      return FilterHeadersStatus::StopIteration;
    }
  }
  ASSERT(http_filter_instance_);
  ENVOY_LOG_MISC(
      info, "[{}] -> envoy_dynamic_module_event_http_filter_instance_request_headers_ ({}, {}, {})",
      dynamic_module_->name_, http_filter_instance_, STATIC_CAST_AS_VOID(&headers), end_of_stream);
  const envoy_dynamic_module_type_EventHttpRequestHeadersStatus result =
      dynamic_module_->envoy_dynamic_module_event_http_filter_instance_request_headers_(
          http_filter_instance_, STATIC_CAST_AS_VOID(&headers), end_of_stream);
  ENVOY_LOG_MISC(info,
                 "[{}] <- envoy_dynamic_module_event_http_filter_instance_request_headers_: {}",
                 dynamic_module_->name_, result);
  this->in_continue_ = result == envoy_dynamic_module_type_EventHttpRequestHeadersStatusContinue;
  return static_cast<FilterHeadersStatus>(result);
};

FilterDataStatus HttpFilter::decodeData(Buffer::Instance& buffer, bool end_of_stream) {
  ASSERT(dynamic_module_);
  ASSERT(http_filter_instance_);
  ENVOY_LOG_MISC(
      info, "[{}] -> envoy_dynamic_module_event_http_filter_instance_request_body_ ({}, {}, {})",
      dynamic_module_->name_, http_filter_instance_, STATIC_CAST_AS_VOID(&buffer), end_of_stream);
  const envoy_dynamic_module_type_EventHttpRequestBodyStatus result =
      dynamic_module_->envoy_dynamic_module_event_http_filter_instance_request_body_(
          http_filter_instance_, STATIC_CAST_AS_VOID(&buffer), end_of_stream);
  ENVOY_LOG_MISC(info, "[{}] <- envoy_dynamic_module_event_http_filter_instance_request_body_: {}",
                 dynamic_module_->name_, result);
  this->in_continue_ = result == envoy_dynamic_module_type_EventHttpRequestBodyStatusContinue;
  return static_cast<FilterDataStatus>(result);
};

FilterHeadersStatus HttpFilter::encodeHeaders(ResponseHeaderMap& headers, bool end_of_stream) {
  ASSERT(dynamic_module_);
  ASSERT(http_filter_instance_);
  ENVOY_LOG_MISC(
      info,
      "[{}] -> envoy_dynamic_module_event_http_filter_instance_response_headers_ ({}, {}, {})",
      dynamic_module_->name_, http_filter_instance_, STATIC_CAST_AS_VOID(&headers), end_of_stream);
  const envoy_dynamic_module_type_EventHttpResponseHeadersStatus result =
      dynamic_module_->envoy_dynamic_module_event_http_filter_instance_response_headers_(
          http_filter_instance_, STATIC_CAST_AS_VOID(&headers), end_of_stream);
  ENVOY_LOG_MISC(info,
                 "[{}] <- envoy_dynamic_module_event_http_filter_instance_response_headers_: {}",
                 dynamic_module_->name_, result);
  this->in_continue_ = result == envoy_dynamic_module_type_EventHttpResponseHeadersStatusContinue;
  return static_cast<FilterHeadersStatus>(result);
};

FilterDataStatus HttpFilter::encodeData(Buffer::Instance& buffer, bool end_of_stream) {
  ASSERT(dynamic_module_);
  ASSERT(http_filter_instance_);
  ENVOY_LOG_MISC(
      info, "[{}] -> envoy_dynamic_module_event_http_filter_instance_response_body_ ({}, {}, {})",
      dynamic_module_->name_, http_filter_instance_, STATIC_CAST_AS_VOID(&buffer), end_of_stream);
  const envoy_dynamic_module_type_EventHttpResponseBodyStatus result =
      dynamic_module_->envoy_dynamic_module_event_http_filter_instance_response_body_(
          http_filter_instance_, STATIC_CAST_AS_VOID(&buffer), end_of_stream);
  ENVOY_LOG_MISC(info, "[{}] <- envoy_dynamic_module_event_http_filter_instance_response_body_: {}",
                 dynamic_module_->name_, result);
  this->in_continue_ = result == envoy_dynamic_module_type_EventHttpResponseBodyStatusContinue;
  return static_cast<FilterDataStatus>(result);
};

} // namespace Http
} // namespace DynamicModules
} // namespace Extensions
} // namespace Envoy
