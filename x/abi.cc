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

size_t __envoy_dynamic_module_http_get_request_header_value(
    ABI::ResponseHeaderMapPtr headers, ABI::InModuleBufferPtr key,
    ABI::InModuleBufferLength key_length, ABI::InModuleBufferPtr* result_buffer_ptr,
    ABI::InModuleBufferLength* result_buffer_length_ptr) {
  RequestHeaderMap* request_headers = static_cast<RequestHeaderMap*>(headers);
  const std::string key_str(static_cast<const char*>(key), key_length);
  const auto header = request_headers->get(Http::LowerCaseString(key_str));

  if (header.empty()) {
    *result_buffer_ptr = nullptr;
    *result_buffer_length_ptr = 0;
    return 0;
  } else {
    const HeaderEntry* entry = header[0];
    const auto entry_length = entry->value().size();
    *result_buffer_ptr = static_cast<ABI::InModuleBufferPtr>(entry->value().getStringView().data());
    *result_buffer_length_ptr = entry_length;
    return header.size();
  }
}

size_t __envoy_dynamic_module_http_get_response_header_value(
    ABI::ResponseHeaderMapPtr headers, ABI::InModuleBufferPtr key,
    ABI::InModuleBufferLength key_length, ABI::InModuleBufferPtr* result_buffer_ptr,
    ABI::InModuleBufferLength* result_buffer_length_ptr) {
  ResponseHeaderMap* response_headers = static_cast<ResponseHeaderMap*>(headers);
  const std::string key_str(static_cast<const char*>(key), key_length);
  const auto header = response_headers->get(Http::LowerCaseString(key_str));

  if (header.empty()) {
    *result_buffer_ptr = nullptr;
    *result_buffer_length_ptr = 0;
    return 0;
  } else {
    const HeaderEntry* entry = header[0];
    const auto entry_length = entry->value().size();
    *result_buffer_ptr = static_cast<ABI::InModuleBufferPtr>(entry->value().getStringView().data());
    *result_buffer_length_ptr = entry_length;
    return header.size();
  }
}

void __envoy_dynamic_module_http_get_request_header_value_nth(
    ABI::ResponseHeaderMapPtr headers, ABI::InModuleBufferPtr key,
    ABI::InModuleBufferLength key_length, ABI::InModuleBufferPtr* result_buffer_ptr,
    ABI::InModuleBufferLength* result_buffer_length_ptr, size_t nth) {
  RequestHeaderMap* request_headers = static_cast<RequestHeaderMap*>(headers);
  const std::string key_str(static_cast<const char*>(key), key_length);
  const auto header = request_headers->get(Http::LowerCaseString(key_str));

  if (nth < 0 || nth >= header.size()) {
    *result_buffer_ptr = nullptr;
    *result_buffer_length_ptr = 0;
    return;
  }

  const HeaderEntry* entry = header[nth];
  const auto entry_length = entry->value().size();
  *result_buffer_ptr = static_cast<ABI::InModuleBufferPtr>(entry->value().getStringView().data());
  *result_buffer_length_ptr = entry_length;
}

void __envoy_dynamic_module_http_get_response_header_value_nth(
    ABI::ResponseHeaderMapPtr headers, ABI::InModuleBufferPtr key,
    ABI::InModuleBufferLength key_length, ABI::InModuleBufferPtr* result_buffer_ptr,
    ABI::InModuleBufferLength* result_buffer_length_ptr, size_t nth) {
  ResponseHeaderMap* response_headers = static_cast<ResponseHeaderMap*>(headers);
  const std::string key_str(static_cast<const char*>(key), key_length);
  const auto header = response_headers->get(Http::LowerCaseString(key_str));

  if (nth < 0 || nth >= header.size()) {
    *result_buffer_ptr = nullptr;
    *result_buffer_length_ptr = 0;
    return;
  }

  const HeaderEntry* entry = header[nth];
  const auto entry_length = entry->value().size();
  *result_buffer_ptr = static_cast<ABI::InModuleBufferPtr>(entry->value().getStringView().data());
  *result_buffer_length_ptr = entry_length;
}

size_t
__envoy_dynamic_module_http_get_request_body_buffer_slices_count(ABI::RequestBufferPtr buffer) {
  Buffer::Instance* request_buffer = static_cast<Buffer::Instance*>(buffer);
  return request_buffer->getRawSlices(std::nullopt).size();
}

void __envoy_dynamic_module_http_get_request_body_buffer_slice(
    ABI::RequestBufferPtr buffer, size_t nth, ABI::DataSlicePtr* result_buffer_ptr,
    ABI::DataSliceLength* result_buffer_length_ptr) {
  Buffer::Instance* request_buffer = static_cast<Buffer::Instance*>(buffer);
  const auto slices = request_buffer->getRawSlices(std::nullopt);
  if (nth < 0 || nth >= slices.size()) {
    *result_buffer_ptr = nullptr;
    *result_buffer_length_ptr = 0;
    return;
  }
  *result_buffer_ptr = static_cast<ABI::DataSlicePtr>(slices[nth].mem_);
  *result_buffer_length_ptr = slices[nth].len_;
}

size_t
__envoy_dynamic_module_http_get_response_body_buffer_slices_count(ABI::ResponseBufferPtr buffer) {
  Buffer::Instance* response_buffer = static_cast<Buffer::Instance*>(buffer);
  return response_buffer->getRawSlices(std::nullopt).size();
}

void __envoy_dynamic_module_http_get_response_body_buffer_slice(
    ABI::ResponseBufferPtr buffer, size_t nth, ABI::DataSlicePtr* result_buffer_ptr,
    ABI::DataSliceLength* result_buffer_length_ptr) {
  Buffer::Instance* response_buffer = static_cast<Buffer::Instance*>(buffer);
  const auto slices = response_buffer->getRawSlices(std::nullopt);
  if (nth < 0 || nth >= slices.size()) {
    *result_buffer_ptr = nullptr;
    *result_buffer_length_ptr = 0;
    return;
  }
  *result_buffer_ptr = static_cast<ABI::DataSlicePtr>(slices[nth].mem_);
  *result_buffer_length_ptr = slices[nth].len_;
}

} // extern "C"

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
