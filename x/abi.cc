#include <filesystem>
#include <optional>
#include <string>
#include <dlfcn.h>

#include "x/filter.h"
#include "x/abi.h"

#include "source/common/common/assert.h"
#include "envoy/common/exception.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {
extern "C" {

#define GET_HEADER_VALUE(header_map_type, request_or_response)                                     \
  const std::string key_str(static_cast<const char*>(key), key_length);                            \
  const auto header = request_or_response##_headers->get(Http::LowerCaseString(key_str));          \
  if (header.empty()) {                                                                            \
    *result_buffer_ptr = nullptr;                                                                  \
    *result_buffer_length_ptr = 0;                                                                 \
    return 0;                                                                                      \
  } else {                                                                                         \
    const HeaderEntry* entry = header[0];                                                          \
    const auto entry_length = entry->value().size();                                               \
    *result_buffer_ptr =                                                                           \
        static_cast<ABI::InModuleBufferPtr>(entry->value().getStringView().data());                \
    *result_buffer_length_ptr = entry_length;                                                      \
    return header.size();                                                                          \
  }

size_t __envoy_dynamic_module_v1_http_get_request_header_value(
    ABI::ResponseHeaderMapPtr headers, ABI::InModuleBufferPtr key,
    ABI::InModuleBufferLength key_length, ABI::InModuleBufferPtr* result_buffer_ptr,
    ABI::InModuleBufferLength* result_buffer_length_ptr) {
  RequestHeaderMap* request_headers = static_cast<RequestHeaderMap*>(headers);
  GET_HEADER_VALUE(RequestHeaderMap, request);
}

size_t __envoy_dynamic_module_v1_http_get_response_header_value(
    ABI::ResponseHeaderMapPtr headers, ABI::InModuleBufferPtr key,
    ABI::InModuleBufferLength key_length, ABI::InModuleBufferPtr* result_buffer_ptr,
    ABI::InModuleBufferLength* result_buffer_length_ptr) {
  ResponseHeaderMap* response_headers = static_cast<ResponseHeaderMap*>(headers);
  GET_HEADER_VALUE(ResponseHeaderMap, response);
}

#define GET_HEADER_VALUE_NTH(header_map_type, request_or_response)                                 \
  const std::string key_str(static_cast<const char*>(key), key_length);                            \
  const auto header = request_or_response##_headers->get(Http::LowerCaseString(key_str));          \
  if (nth < 0 || nth >= header.size()) {                                                           \
    *result_buffer_ptr = nullptr;                                                                  \
    *result_buffer_length_ptr = 0;                                                                 \
    return;                                                                                        \
  }                                                                                                \
  const HeaderEntry* entry = header[nth];                                                          \
  const auto entry_length = entry->value().size();                                                 \
  *result_buffer_ptr = static_cast<ABI::InModuleBufferPtr>(entry->value().getStringView().data()); \
  *result_buffer_length_ptr = entry_length;

void __envoy_dynamic_module_v1_http_get_request_header_value_nth(
    ABI::ResponseHeaderMapPtr headers, ABI::InModuleBufferPtr key,
    ABI::InModuleBufferLength key_length, ABI::InModuleBufferPtr* result_buffer_ptr,
    ABI::InModuleBufferLength* result_buffer_length_ptr, size_t nth) {
  RequestHeaderMap* request_headers = static_cast<RequestHeaderMap*>(headers);
  GET_HEADER_VALUE_NTH(RequestHeaderMap, request);
}

void __envoy_dynamic_module_v1_http_get_response_header_value_nth(
    ABI::ResponseHeaderMapPtr headers, ABI::InModuleBufferPtr key,
    ABI::InModuleBufferLength key_length, ABI::InModuleBufferPtr* result_buffer_ptr,
    ABI::InModuleBufferLength* result_buffer_length_ptr, size_t nth) {
  ResponseHeaderMap* response_headers = static_cast<ResponseHeaderMap*>(headers);
  GET_HEADER_VALUE_NTH(ResponseHeaderMap, response);
}

#define GET_BUFFER_SLICES_COUNT(buffer_ptr)                                                        \
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer_ptr);                          \
  return _buffer->getRawSlices(std::nullopt).size();

size_t
__envoy_dynamic_module_v1_http_get_request_body_buffer_slices_count(ABI::RequestBufferPtr buffer) {
  GET_BUFFER_SLICES_COUNT(buffer);
}

size_t __envoy_dynamic_module_v1_http_get_response_body_buffer_slices_count(
    ABI::ResponseBufferPtr buffer) {
  GET_BUFFER_SLICES_COUNT(buffer);
}

#define GET_BUFFER_SLICE(buffer_ptr, nth)                                                          \
  Buffer::Instance* _buffer = static_cast<Buffer::Instance*>(buffer_ptr);                          \
  const auto slices = _buffer->getRawSlices(std::nullopt);                                         \
  if (nth < 0 || nth >= slices.size()) {                                                           \
    *result_buffer_ptr = nullptr;                                                                  \
    *result_buffer_length_ptr = 0;                                                                 \
    return;                                                                                        \
  }                                                                                                \
  *result_buffer_ptr = static_cast<ABI::DataSlicePtr>(slices[nth].mem_);                           \
  *result_buffer_length_ptr = slices[nth].len_;

void __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(
    ABI::RequestBufferPtr buffer, size_t nth, ABI::DataSlicePtr* result_buffer_ptr,
    ABI::DataSliceLength* result_buffer_length_ptr) {
  GET_BUFFER_SLICE(buffer, nth);
}

void __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(
    ABI::ResponseBufferPtr buffer, size_t nth, ABI::DataSlicePtr* result_buffer_ptr,
    ABI::DataSliceLength* result_buffer_length_ptr) {
  GET_BUFFER_SLICE(buffer, nth);
}

#define SET_HEADER_VALUE(header_map_type, request_or_response)                                     \
  const std::string key_str(static_cast<const char*>(key), key_length);                            \
  if (value == nullptr) {                                                                          \
    request_or_response##_headers->remove(Http::LowerCaseString(key_str));                         \
    return;                                                                                        \
  }                                                                                                \
  const std::string value_str(static_cast<const char*>(value), value_length);                      \
  request_or_response##_headers->setCopy(Http::LowerCaseString(key_str), value_str);

void __envoy_dynamic_module_v1_http_set_request_header(ABI::RequestHeadersPtr headers,
                                                       ABI::InModuleBufferPtr key,
                                                       ABI::InModuleBufferLength key_length,
                                                       ABI::InModuleBufferPtr value,
                                                       ABI::InModuleBufferLength value_length) {
  RequestHeaderMap* request_headers = static_cast<RequestHeaderMap*>(headers);
  SET_HEADER_VALUE(RequestHeaderMap, request);
}

void __envoy_dynamic_module_v1_http_set_response_header(ABI::ResponseHeaderMapPtr headers,
                                                        ABI::InModuleBufferPtr key,
                                                        ABI::InModuleBufferLength key_length,
                                                        ABI::InModuleBufferPtr value,
                                                        ABI::InModuleBufferLength value_length) {
  ResponseHeaderMap* response_headers = static_cast<ResponseHeaderMap*>(headers);
  SET_HEADER_VALUE(ResponseHeaderMap, response);
}

} // extern "C"

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
