#include "gtest/gtest.h"
#include <memory>

#include "x/abi.h"
#include "x/filter.h"

#include "test/test_common/utility.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

TEST(TestABI, GetRequestHeaderValueEmpty) {
  Http::TestRequestHeaderMapImpl request_headers{};
  ABI::ResponseHeaderMapPtr headers = &request_headers;
  const char* key = "key";
  ABI::InModuleBufferPtr key_ptr = reinterpret_cast<ABI::InModuleBufferPtr>(key);
  ABI::InModuleBufferLength key_length = strlen(key);
  ABI::InModuleBufferPtr result_buffer_ptr;
  ABI::InModuleBufferLength result_buffer_length_ptr;
  int num_values = ABI::__envoy_dynamic_module_http_get_request_header_value(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr);
  EXPECT_EQ(num_values, 0);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length_ptr, 0);
}

TEST(TestABI, GetResponseHeaderValueEmpty) {
  Http::TestResponseHeaderMapImpl response_headers{};
  ABI::ResponseHeaderMapPtr headers = &response_headers;
  const char* key = "key";
  ABI::InModuleBufferPtr key_ptr = reinterpret_cast<ABI::InModuleBufferPtr>(key);
  ABI::InModuleBufferLength key_length = strlen(key);
  ABI::InModuleBufferPtr result_buffer_ptr;
  ABI::InModuleBufferLength result_buffer_length_ptr;
  int num_values = ABI::__envoy_dynamic_module_http_get_response_header_value(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr);
  EXPECT_EQ(num_values, 0);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length_ptr, 0);
}

TEST(TestABI, GetRequestHeaderValue) {
  Http::TestRequestHeaderMapImpl request_headers{{":path", "/"}, {"key", "value"}};
  ABI::ResponseHeaderMapPtr headers = &request_headers;

  const char* key = "key";
  ABI::InModuleBufferPtr key_ptr = reinterpret_cast<ABI::InModuleBufferPtr>(key);
  ABI::InModuleBufferLength key_length = strlen(key);
  ABI::InModuleBufferPtr result_buffer_ptr;
  ABI::InModuleBufferLength result_buffer_length_ptr;
  int num_values = ABI::__envoy_dynamic_module_http_get_request_header_value(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr);
  EXPECT_EQ(num_values, 1);
  // Read the value from the buffer.
  std::string result = std::string(result_buffer_ptr, result_buffer_length_ptr);
  EXPECT_EQ(result, "value");

  // Test different key.
  const char* key2 = ":path";
  ABI::InModuleBufferPtr key_ptr2 = reinterpret_cast<ABI::InModuleBufferPtr>(key2);
  ABI::InModuleBufferLength key_length2 = strlen(key2);
  ABI::InModuleBufferPtr result_buffer_ptr2;
  ABI::InModuleBufferLength result_buffer_length_ptr2;
  int num_values2 = ABI::__envoy_dynamic_module_http_get_request_header_value(
      headers, key_ptr2, key_length2, &result_buffer_ptr2, &result_buffer_length_ptr2);
  EXPECT_EQ(num_values2, 1);
  // Read the value from the buffer.
  std::string result2 = std::string(result_buffer_ptr2, result_buffer_length_ptr2);
  EXPECT_EQ(result2, "/");
}

TEST(TestABI, GetResponseHeaderValue) {
  Http::TestResponseHeaderMapImpl response_headers{{"foo", "bar"}};
  ABI::ResponseHeaderMapPtr headers = &response_headers;
  const char* key = "foo";
  ABI::InModuleBufferPtr key_ptr = reinterpret_cast<ABI::InModuleBufferPtr>(key);
  ABI::InModuleBufferLength key_length = strlen(key);
  ABI::InModuleBufferPtr result_buffer_ptr;
  ABI::InModuleBufferLength result_buffer_length_ptr;
  int num_values = ABI::__envoy_dynamic_module_http_get_response_header_value(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr);
  EXPECT_EQ(num_values, 1);
  // Read the value from the buffer.
  std::string result = std::string(result_buffer_ptr, result_buffer_length_ptr);
  EXPECT_EQ(result, "bar");

  // Test different key.
  const char* key2 = "non-existent-key";
  ABI::InModuleBufferPtr key_ptr2 = reinterpret_cast<ABI::InModuleBufferPtr>(key2);
  ABI::InModuleBufferLength key_length2 = strlen(key2);
  ABI::InModuleBufferPtr result_buffer_ptr2;
  ABI::InModuleBufferLength result_buffer_length_ptr2;
  int num_values2 = ABI::__envoy_dynamic_module_http_get_response_header_value(
      headers, key_ptr2, key_length2, &result_buffer_ptr2, &result_buffer_length_ptr2);
  EXPECT_EQ(num_values2, 0);
  EXPECT_EQ(result_buffer_ptr2, nullptr);
  EXPECT_EQ(result_buffer_length_ptr2, 0);
}

TEST(TestABI, GetRequestHeaderValueNth) {
  Http::TestRequestHeaderMapImpl request_headers{};
  request_headers.addCopy(LowerCaseString("key"), "value1");
  request_headers.addCopy(LowerCaseString("key"), "value2");
  ABI::ResponseHeaderMapPtr headers = &request_headers;
  const char* key = "key";
  ABI::InModuleBufferPtr key_ptr = reinterpret_cast<ABI::InModuleBufferPtr>(key);
  ABI::InModuleBufferLength key_length = strlen(key);
  ABI::InModuleBufferPtr result_buffer_ptr;
  ABI::InModuleBufferLength result_buffer_length_ptr;
  ABI::__envoy_dynamic_module_http_get_request_header_value_nth(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr, 1);
  // Read the value from the buffer.
  std::string result = std::string(result_buffer_ptr, result_buffer_length_ptr);
  EXPECT_EQ(result, "value2");
}

TEST(TestABI, GetResponseHeaderValueNth) {
  Http::TestResponseHeaderMapImpl response_headers{};
  response_headers.addCopy(LowerCaseString("key"), "value1");
  response_headers.addCopy(LowerCaseString("key"), "value2");
  ABI::ResponseHeaderMapPtr headers = &response_headers;
  const char* key = "key";
  ABI::InModuleBufferPtr key_ptr = reinterpret_cast<ABI::InModuleBufferPtr>(key);
  ABI::InModuleBufferLength key_length = strlen(key);
  ABI::InModuleBufferPtr result_buffer_ptr;
  ABI::InModuleBufferLength result_buffer_length_ptr;
  ABI::__envoy_dynamic_module_http_get_response_header_value_nth(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr, 1);
  // Read the value from the buffer.
  std::string result = std::string(result_buffer_ptr, result_buffer_length_ptr);
  EXPECT_EQ(result, "value2");
}

TEST(TestABI, GetRequestHeaderValueNthOutOfBounds) {
  Http::TestRequestHeaderMapImpl request_headers{};
  request_headers.addCopy(LowerCaseString("key"), "value1");
  ABI::ResponseHeaderMapPtr headers = &request_headers;
  const char* key = "key";
  ABI::InModuleBufferPtr key_ptr = reinterpret_cast<ABI::InModuleBufferPtr>(key);
  ABI::InModuleBufferLength key_length = strlen(key);
  ABI::InModuleBufferPtr result_buffer_ptr;
  ABI::InModuleBufferLength result_buffer_length_ptr;
  ABI::__envoy_dynamic_module_http_get_request_header_value_nth(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr, 1);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length_ptr, 0);
}

TEST(TestABI, GetResponseHeaderValueNthOutOfBounds) {
  Http::TestResponseHeaderMapImpl response_headers{};
  response_headers.addCopy(LowerCaseString("key"), "value1");
  ABI::ResponseHeaderMapPtr headers = &response_headers;
  const char* key = "key";
  ABI::InModuleBufferPtr key_ptr = reinterpret_cast<ABI::InModuleBufferPtr>(key);
  ABI::InModuleBufferLength key_length = strlen(key);
  ABI::InModuleBufferPtr result_buffer_ptr;
  ABI::InModuleBufferLength result_buffer_length_ptr;
  ABI::__envoy_dynamic_module_http_get_response_header_value_nth(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr, 1);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length_ptr, 0);
}

TEST(TestABIRoundTrip, GetHeaders) {
  DynamicModuleSharedPtr module = std::make_shared<DynamicModule>(
      "./test/test_programs/libget_headers.so", "config", "TestABIRoundTripGetHeaders");
  auto filter = std::make_shared<HttpFilter>(module);

  Http::TestRequestHeaderMapImpl request_headers{{"key", "value"}};
  const auto result = filter->decodeHeaders(request_headers, false);
  EXPECT_EQ(result, FilterHeadersStatus::Continue);

  Http::TestResponseHeaderMapImpl response_headers{{"foo", "bar"}};
  const auto result2 = filter->encodeHeaders(response_headers, false);
  EXPECT_EQ(result2, FilterHeadersStatus::Continue);
}

TEST(TestABI, GetRequestBodyBufferSlicesCount) {
  Buffer::OwnedImpl body;
  EXPECT_EQ(ABI::__envoy_dynamic_module_http_get_request_body_buffer_slices_count(&body), 0);

  body.appendSliceForTest("hello");
  EXPECT_EQ(ABI::__envoy_dynamic_module_http_get_request_body_buffer_slices_count(&body), 1);

  body.appendSliceForTest("world");
  EXPECT_EQ(ABI::__envoy_dynamic_module_http_get_request_body_buffer_slices_count(&body), 2);
}

TEST(TestABI, GetResponseBodyBufferSlicesCount) {
  Buffer::OwnedImpl body;
  EXPECT_EQ(ABI::__envoy_dynamic_module_http_get_response_body_buffer_slices_count(&body), 0);

  body.appendSliceForTest("hello");
  EXPECT_EQ(ABI::__envoy_dynamic_module_http_get_response_body_buffer_slices_count(&body), 1);

  body.appendSliceForTest("world");
  EXPECT_EQ(ABI::__envoy_dynamic_module_http_get_response_body_buffer_slices_count(&body), 2);
}

TEST(TestABI, GetRequestBodyBufferSlice) {
  Buffer::OwnedImpl body;
  body.appendSliceForTest("hello");
  body.appendSliceForTest("world");

  ABI::InModuleBufferPtr result_buffer_ptr;
  ABI::InModuleBufferLength result_buffer_length;
  ABI::__envoy_dynamic_module_http_get_request_body_buffer_slice(&body, 0, &result_buffer_ptr,
                                                                 &result_buffer_length);
  std::string result = std::string(result_buffer_ptr, result_buffer_length);
  EXPECT_EQ(result, "hello");

  ABI::__envoy_dynamic_module_http_get_request_body_buffer_slice(&body, 1, &result_buffer_ptr,
                                                                 &result_buffer_length);
  std::string result2 = std::string(result_buffer_ptr, result_buffer_length);
  EXPECT_EQ(result2, "world");

  ABI::__envoy_dynamic_module_http_get_request_body_buffer_slice(&body, 2, &result_buffer_ptr,
                                                                 &result_buffer_length);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length, 0);
}

TEST(TestABI, GetResponseBodyBufferSlice) {
  Buffer::OwnedImpl body;
  body.appendSliceForTest("hello");
  body.appendSliceForTest("world");

  ABI::InModuleBufferPtr result_buffer_ptr;
  ABI::InModuleBufferLength result_buffer_length;
  ABI::__envoy_dynamic_module_http_get_response_body_buffer_slice(&body, 0, &result_buffer_ptr,
                                                                  &result_buffer_length);
  std::string result = std::string(result_buffer_ptr, result_buffer_length);
  EXPECT_EQ(result, "hello");

  ABI::__envoy_dynamic_module_http_get_response_body_buffer_slice(&body, 1, &result_buffer_ptr,
                                                                  &result_buffer_length);
  std::string result2 = std::string(result_buffer_ptr, result_buffer_length);
  EXPECT_EQ(result2, "world");

  ABI::__envoy_dynamic_module_http_get_response_body_buffer_slice(&body, 2, &result_buffer_ptr,
                                                                  &result_buffer_length);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length, 0);
}

TEST(TestABIRoundTrip, GetBody) {
  DynamicModuleSharedPtr module = std::make_shared<DynamicModule>(
      "./test/test_programs/libget_body.so", "config", "TestABIRoundTripGetHeaders");
  auto filter = std::make_shared<HttpFilter>(module);
  filter->ensureStreamContext();

  Buffer::OwnedImpl request_body;
  request_body.appendSliceForTest("hello");
  request_body.appendSliceForTest("world");
  const auto result = filter->decodeData(request_body, false);
  EXPECT_EQ(result, FilterDataStatus::Continue);

  Buffer::OwnedImpl response_body;
  response_body.appendSliceForTest("Go");
  response_body.appendSliceForTest("Rust");
  const auto result2 = filter->encodeData(response_body, false);
  EXPECT_EQ(result2, FilterDataStatus::Continue);
}

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
