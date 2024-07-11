#include <string>

#include "filter.h"

#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

HttpFilter::HttpFilter(DynamicModuleSharedPtr config) : config_(config) {}

HttpFilter::~HttpFilter() = default;

void HttpFilter::onStreamComplete(){};

void HttpFilter::onDestroy(){};

FilterHeadersStatus HttpFilter::decodeHeaders(RequestHeaderMap&, bool) {
  return FilterHeadersStatus::Continue;
};

FilterDataStatus HttpFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
};

FilterTrailersStatus HttpFilter::decodeTrailers(RequestTrailerMap&) {
  return FilterTrailersStatus::Continue;
};

FilterMetadataStatus HttpFilter::decodeMetadata(MetadataMap&) {
  return FilterMetadataStatus::Continue;
};

void HttpFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
};

void HttpFilter::decodeComplete(){};

Filter1xxHeadersStatus HttpFilter::encode1xxHeaders(ResponseHeaderMap&) {
  return Filter1xxHeadersStatus::Continue;
};

FilterHeadersStatus HttpFilter::encodeHeaders(ResponseHeaderMap&, bool) {
  return FilterHeadersStatus::Continue;
};

FilterDataStatus HttpFilter::encodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
};

FilterTrailersStatus HttpFilter::encodeTrailers(ResponseTrailerMap&) {
  return FilterTrailersStatus::Continue;
};

FilterMetadataStatus HttpFilter::encodeMetadata(MetadataMap&) {
  return FilterMetadataStatus::Continue;
};

void HttpFilter::setEncoderFilterCallbacks(StreamEncoderFilterCallbacks& callbacks) {
  encoder_callbacks_ = &callbacks;
};

void HttpFilter::encodeComplete(){};

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy