
#pragma once

#include <string>

#include "source/extensions/filters/http/common/pass_through_filter.h"

#include "x/config.pb.h"
#include "x/dynamic_module.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

using DynamicModuleSharedPtr = std::shared_ptr<DynamicModule>;

class HttpFilter : public Http::StreamFilter {
public:
  HttpFilter(DynamicModuleSharedPtr);
  ~HttpFilter() override;

  // ---------- Http::StreamFilterBase ------------
  /**
   * This routine is called before the access log handlers' final log() is called. Filters can use
   * this callback to enrich the data passed in to the log handlers.
   */
  void onStreamComplete() override;

  /**
   * This routine is called prior to a filter being destroyed. This may happen after normal stream
   * finish (both downstream and upstream) or due to reset. Every filter is responsible for making
   * sure that any async events are cleaned up in the context of this routine. This includes timers,
   * network calls, etc. The reason there is an onDestroy() method vs. doing this type of cleanup
   * in the destructor is due to the deferred deletion model that Envoy uses to avoid stack unwind
   * complications. Filters must not invoke either encoder or decoder filter callbacks after having
   * onDestroy() invoked. Filters that cross-register as access log handlers receive log() before
   * onDestroy().
   */
  void onDestroy() override;

  // ----------  Http::StreamDecoderFilter  ----------
  /**
   * Called with decoded headers, optionally indicating end of stream.
   * @param headers supplies the decoded headers map.
   * @param end_stream supplies whether this is a header only request/response.
   * @return FilterHeadersStatus determines how filter chain iteration proceeds.
   */
  FilterHeadersStatus decodeHeaders(RequestHeaderMap& headers, bool end_stream) override;

  /**
   * Called with a decoded data frame.
   * @param data supplies the decoded data.
   * @param end_stream supplies whether this is the last data frame.
   * Further note that end_stream is only true if there are no trailers.
   * @return FilterDataStatus determines how filter chain iteration proceeds.
   */
  FilterDataStatus decodeData(Buffer::Instance& data, bool end_stream) override;

  /**
   * Called with decoded trailers, implicitly ending the stream.
   * @param trailers supplies the decoded trailers.
   */
  FilterTrailersStatus decodeTrailers(RequestTrailerMap& trailers) override;

  /**
   * Called with decoded metadata. Add new metadata to metadata_map directly. Do not call
   * StreamDecoderFilterCallbacks::addDecodedMetadata() to add new metadata.
   *
   * Note: decodeMetadata() currently cannot stop the filter iteration, and always returns Continue.
   * That means metadata will go through the complete filter chain at once, even if the other frame
   * types return StopIteration. If metadata should not pass through all filters at once, users
   * should consider using StopAllIterationAndBuffer or StopAllIterationAndWatermark in
   * decodeHeaders() to prevent metadata passing to the following filters.
   *
   * @param metadata_map supplies the decoded metadata.
   */
  FilterMetadataStatus decodeMetadata(MetadataMap&) override;

  /**
   * Called by the filter manager once to initialize the filter decoder callbacks that the
   * filter should use. Callbacks will not be invoked by the filter after onDestroy() is called.
   */
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) override;

  /**
   * Called at the end of the stream, when all data has been decoded.
   */
  void decodeComplete() override;

  // ----------  Http::StreamEncoderFilter  ----------

  /**
   * Called with supported 1xx headers.
   *
   * This is not folded into encodeHeaders because most Envoy users and filters
   * will not be proxying 1xxs and with it split out, can ignore the
   * complexity of multiple encodeHeaders calls.
   *
   * This will only be invoked once per request.
   *
   * @param headers supplies the 1xx response headers to be encoded.
   * @return Filter1xxHeadersStatus determines how filter chain iteration proceeds.
   *
   */
  Filter1xxHeadersStatus encode1xxHeaders(ResponseHeaderMap& headers) override;

  /**
   * Called with headers to be encoded, optionally indicating end of stream.
   *
   * The only 1xx that may be provided to encodeHeaders() is a 101 upgrade, which will be the final
   * encodeHeaders() for a response.
   *
   * @param headers supplies the headers to be encoded.
   * @param end_stream supplies whether this is a header only request/response.
   * @return FilterHeadersStatus determines how filter chain iteration proceeds.
   */
  FilterHeadersStatus encodeHeaders(ResponseHeaderMap& headers, bool end_stream) override;

  /**
   * Called with data to be encoded, optionally indicating end of stream.
   * @param data supplies the data to be encoded.
   * @param end_stream supplies whether this is the last data frame.
   * Further note that end_stream is only true if there are no trailers.
   * @return FilterDataStatus determines how filter chain iteration proceeds.
   */
  FilterDataStatus encodeData(Buffer::Instance& data, bool end_stream) override;

  /**
   * Called with trailers to be encoded, implicitly ending the stream.
   * @param trailers supplies the trailers to be encoded.
   */
  FilterTrailersStatus encodeTrailers(ResponseTrailerMap& trailers) override;

  /**
   * Called with metadata to be encoded. New metadata should be added directly to metadata_map. DO
   * NOT call StreamDecoderFilterCallbacks::encodeMetadata() interface to add new metadata.
   *
   * @param metadata_map supplies the metadata to be encoded.
   * @return FilterMetadataStatus, which currently is always FilterMetadataStatus::Continue;
   */
  FilterMetadataStatus encodeMetadata(MetadataMap& metadata_map) override;

  /**
   * Called by the filter manager once to initialize the filter callbacks that the filter should
   * use. Callbacks will not be invoked by the filter after onDestroy() is called.
   */
  void setEncoderFilterCallbacks(StreamEncoderFilterCallbacks& callbacks) override;

  /**
   * Called at the end of the stream, when all data has been encoded.
   */
  void encodeComplete() override;

private:
  const DynamicModuleSharedPtr config_;
  StreamDecoderFilterCallbacks* decoder_callbacks_;
  StreamEncoderFilterCallbacks* encoder_callbacks_;

  const LowerCaseString headerKey() const;
  const std::string headerValue() const;
};

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
