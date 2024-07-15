#include <filesystem>
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

size_t __envoy_dynamic_module_get_request_header_value(
    ABI::ResponseHeaderMapPtr headers, ABI::InModuleBufferPtr key,
    ABI::InModuleBufferLength key_length, ABI::InModuleBufferPtr* result_buffer_ptr,
    ABI::InModuleBufferLength* result_buffer_length_ptr) {
  RequestHeaderMap& request_headers = *reinterpret_cast<RequestHeaderMap*>(headers);
  const std::string key_str(reinterpret_cast<const char*>(key), key_length);
  const auto header = request_headers.get(Http::LowerCaseString(key_str));

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

void __envoy_dynamic_module_get_request_header_value_nth(
    ABI::ResponseHeaderMapPtr headers, ABI::InModuleBufferPtr key,
    ABI::InModuleBufferLength key_length, ABI::InModuleBufferPtr* result_buffer_ptr,
    ABI::InModuleBufferLength* result_buffer_length_ptr, size_t nth) {
  RequestHeaderMap& request_headers = *reinterpret_cast<RequestHeaderMap*>(headers);
  const std::string key_str(reinterpret_cast<const char*>(key), key_length);
  const auto header = request_headers.get(Http::LowerCaseString(key_str));

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
}

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
