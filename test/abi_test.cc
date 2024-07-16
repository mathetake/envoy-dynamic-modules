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
  __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers = &request_headers;
  const char* key = "key";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = strlen(key);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length_ptr;
  int num_values = __envoy_dynamic_module_v1_http_get_request_header_value(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr);
  EXPECT_EQ(num_values, 0);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length_ptr, 0);
}

TEST(TestABI, GetResponseHeaderValueEmpty) {
  Http::TestResponseHeaderMapImpl response_headers{};
  __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers = &response_headers;
  const char* key = "key";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = strlen(key);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length_ptr;
  int num_values = __envoy_dynamic_module_v1_http_get_response_header_value(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr);
  EXPECT_EQ(num_values, 0);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length_ptr, 0);
}

TEST(TestABI, GetRequestHeaderValue) {
  Http::TestRequestHeaderMapImpl request_headers{{":path", "/"}, {"key", "value"}};
  __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers = &request_headers;

  const char* key = "key";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = strlen(key);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length_ptr;
  int num_values = __envoy_dynamic_module_v1_http_get_request_header_value(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr);
  EXPECT_EQ(num_values, 1);
  // Read the value from the buffer.
  std::string result = std::string(result_buffer_ptr, result_buffer_length_ptr);
  EXPECT_EQ(result, "value");

  // Test different key.
  const char* key2 = ":path";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr2 =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key2);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length2 = strlen(key2);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr2;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length_ptr2;
  int num_values2 = __envoy_dynamic_module_v1_http_get_request_header_value(
      headers, key_ptr2, key_length2, &result_buffer_ptr2, &result_buffer_length_ptr2);
  EXPECT_EQ(num_values2, 1);
  // Read the value from the buffer.
  std::string result2 = std::string(result_buffer_ptr2, result_buffer_length_ptr2);
  EXPECT_EQ(result2, "/");
}

TEST(TestABI, GetResponseHeaderValue) {
  Http::TestResponseHeaderMapImpl response_headers{{"foo", "bar"}};
  __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers = &response_headers;
  const char* key = "foo";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = strlen(key);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length_ptr;
  int num_values = __envoy_dynamic_module_v1_http_get_response_header_value(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr);
  EXPECT_EQ(num_values, 1);
  // Read the value from the buffer.
  std::string result = std::string(result_buffer_ptr, result_buffer_length_ptr);
  EXPECT_EQ(result, "bar");

  // Test different key.
  const char* key2 = "non-existent-key";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr2 =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key2);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length2 = strlen(key2);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr2;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length_ptr2;
  int num_values2 = __envoy_dynamic_module_v1_http_get_response_header_value(
      headers, key_ptr2, key_length2, &result_buffer_ptr2, &result_buffer_length_ptr2);
  EXPECT_EQ(num_values2, 0);
  EXPECT_EQ(result_buffer_ptr2, nullptr);
  EXPECT_EQ(result_buffer_length_ptr2, 0);
}

TEST(TestABI, GetRequestHeaderValueNth) {
  Http::TestRequestHeaderMapImpl request_headers{};
  request_headers.addCopy(LowerCaseString("key"), "value1");
  request_headers.addCopy(LowerCaseString("key"), "value2");
  __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers = &request_headers;
  const char* key = "key";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = strlen(key);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length_ptr;
  __envoy_dynamic_module_v1_http_get_request_header_value_nth(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr, 1);
  // Read the value from the buffer.
  std::string result = std::string(result_buffer_ptr, result_buffer_length_ptr);
  EXPECT_EQ(result, "value2");
}

TEST(TestABI, GetResponseHeaderValueNth) {
  Http::TestResponseHeaderMapImpl response_headers{};
  response_headers.addCopy(LowerCaseString("key"), "value1");
  response_headers.addCopy(LowerCaseString("key"), "value2");
  __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers = &response_headers;
  const char* key = "key";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = strlen(key);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length_ptr;
  __envoy_dynamic_module_v1_http_get_response_header_value_nth(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr, 1);
  // Read the value from the buffer.
  std::string result = std::string(result_buffer_ptr, result_buffer_length_ptr);
  EXPECT_EQ(result, "value2");
}

TEST(TestABI, GetRequestHeaderValueNthOutOfBounds) {
  Http::TestRequestHeaderMapImpl request_headers{};
  request_headers.addCopy(LowerCaseString("key"), "value1");
  __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers = &request_headers;
  const char* key = "key";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = strlen(key);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length_ptr;
  __envoy_dynamic_module_v1_http_get_request_header_value_nth(
      headers, key_ptr, key_length, &result_buffer_ptr, &result_buffer_length_ptr, 1);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length_ptr, 0);
}

TEST(TestABI, GetResponseHeaderValueNthOutOfBounds) {
  Http::TestResponseHeaderMapImpl response_headers{};
  response_headers.addCopy(LowerCaseString("key"), "value1");
  __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers = &response_headers;
  const char* key = "key";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = strlen(key);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length_ptr;
  __envoy_dynamic_module_v1_http_get_response_header_value_nth(
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
  EXPECT_EQ(__envoy_dynamic_module_v1_http_get_request_body_buffer_slices_count(&body), 0);

  body.appendSliceForTest("hello");
  EXPECT_EQ(__envoy_dynamic_module_v1_http_get_request_body_buffer_slices_count(&body), 1);

  body.appendSliceForTest("world");
  EXPECT_EQ(__envoy_dynamic_module_v1_http_get_request_body_buffer_slices_count(&body), 2);
}

TEST(TestABI, GetResponseBodyBufferSlicesCount) {
  Buffer::OwnedImpl body;
  EXPECT_EQ(__envoy_dynamic_module_v1_http_get_response_body_buffer_slices_count(&body), 0);

  body.appendSliceForTest("hello");
  EXPECT_EQ(__envoy_dynamic_module_v1_http_get_response_body_buffer_slices_count(&body), 1);

  body.appendSliceForTest("world");
  EXPECT_EQ(__envoy_dynamic_module_v1_http_get_response_body_buffer_slices_count(&body), 2);
}

TEST(TestABI, GetRequestBodyBufferSlice) {
  Buffer::OwnedImpl body;
  body.appendSliceForTest("hello");
  body.appendSliceForTest("world");

  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length;
  __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(&body, 0, &result_buffer_ptr,
                                                               &result_buffer_length);
  std::string result = std::string(result_buffer_ptr, result_buffer_length);
  EXPECT_EQ(result, "hello");

  __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(&body, 1, &result_buffer_ptr,
                                                               &result_buffer_length);
  std::string result2 = std::string(result_buffer_ptr, result_buffer_length);
  EXPECT_EQ(result2, "world");

  __envoy_dynamic_module_v1_http_get_request_body_buffer_slice(&body, 2, &result_buffer_ptr,
                                                               &result_buffer_length);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length, 0);
}

TEST(TestABI, GetResponseBodyBufferSlice) {
  Buffer::OwnedImpl body;
  body.appendSliceForTest("hello");
  body.appendSliceForTest("world");

  __envoy_dynamic_module_v1_type_InModuleBufferPtr result_buffer_ptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength result_buffer_length;
  __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(&body, 0, &result_buffer_ptr,
                                                                &result_buffer_length);
  std::string result = std::string(result_buffer_ptr, result_buffer_length);
  EXPECT_EQ(result, "hello");

  __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(&body, 1, &result_buffer_ptr,
                                                                &result_buffer_length);
  std::string result2 = std::string(result_buffer_ptr, result_buffer_length);
  EXPECT_EQ(result2, "world");

  __envoy_dynamic_module_v1_http_get_response_body_buffer_slice(&body, 2, &result_buffer_ptr,
                                                                &result_buffer_length);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length, 0);
}

TEST(TestABIRoundTrip, GetBody) {
  DynamicModuleSharedPtr module = std::make_shared<DynamicModule>(
      "./test/test_programs/libget_body.so", "config", "TestABIRoundTripGetBody");
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

TEST(TestABI, SetHeader_Add) {
  Http::TestRequestHeaderMapImpl request_headers{};
  __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers = &request_headers;
  const char key[] = "key";
  const char value[] = "value";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = strlen(key);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr value_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(value);
  __envoy_dynamic_module_v1_type_InModuleBufferLength value_length = strlen(value);
  __envoy_dynamic_module_v1_http_set_request_header(headers, key_ptr, key_length, value_ptr,
                                                    value_length);
  EXPECT_EQ(request_headers.get(LowerCaseString(key))[0]->value().getStringView(), value);
}

TEST(TestABI, SetHeader_Add_empty_value) {
  Http::TestRequestHeaderMapImpl request_headers{};
  __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers = &request_headers;
  const char key[] = "key";
  const char value[] = "";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = strlen(key);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr value_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(value);
  __envoy_dynamic_module_v1_type_InModuleBufferLength value_length = strlen(value);
  __envoy_dynamic_module_v1_http_set_request_header(headers, key_ptr, key_length, value_ptr,
                                                    value_length);
  EXPECT_EQ(request_headers.get(LowerCaseString(key))[0]->value().getStringView(), value);
}

TEST(TestABI, SetHeader_Remove) {
  Http::TestRequestHeaderMapImpl request_headers{{"key", "value"}};
  __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers = &request_headers;
  const char key[] = "key";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = strlen(key);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr value_ptr = nullptr;
  __envoy_dynamic_module_v1_type_InModuleBufferLength value_length = 0;
  __envoy_dynamic_module_v1_http_set_request_header(headers, key_ptr, key_length, value_ptr,
                                                    value_length);
  EXPECT_EQ(request_headers.size(), 0);
}

TEST(TestABI, SetHeader_Replaces) {
  Http::TestRequestHeaderMapImpl request_headers{{"key", "value"}};
  __envoy_dynamic_module_v1_type_HttpResponseHeaderMapPtr headers = &request_headers;
  const char key[] = "key";
  const char value[] = "new_value";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(key);
  __envoy_dynamic_module_v1_type_InModuleBufferLength key_length = strlen(key);
  __envoy_dynamic_module_v1_type_InModuleBufferPtr value_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(value);
  __envoy_dynamic_module_v1_type_InModuleBufferLength value_length = strlen(value);
  __envoy_dynamic_module_v1_http_set_request_header(headers, key_ptr, key_length, value_ptr,
                                                    value_length);
  EXPECT_EQ(request_headers.get(LowerCaseString(key))[0]->value().getStringView(), value);
}

TEST(TestABIRoundTrip, SetHeaders) {
  DynamicModuleSharedPtr module = std::make_shared<DynamicModule>(
      "./test/test_programs/libset_headers.so", "config", "TestABIRoundTripSetHeaders");
  auto filter = std::make_shared<HttpFilter>(module);

  Http::TestRequestHeaderMapImpl request_headers{
      {
          "existing_key",
          "old",
      },
      {
          "existing_key_with_multiple_values",
          "1",
      },
      {
          "existing_key_with_multiple_values",
          "2",
      },
      {
          "to_delete",
          "old",
      },
  };

  const auto result = filter->decodeHeaders(request_headers, false);
  EXPECT_EQ(result, FilterHeadersStatus::Continue);
  Http::TestResponseHeaderMapImpl response_headers{
      {
          "existing_key",
          "old",
      },
      {
          "existing_key_with_multiple_values",
          "1",
      },
      {
          "existing_key_with_multiple_values",
          "2",
      },
      {
          "to_delete",
          "old",
      },
  };
  const auto result2 = filter->encodeHeaders(response_headers, false);
  EXPECT_EQ(result2, FilterHeadersStatus::Continue);
  // Print the headers to check the values.
  response_headers.iterate([&](const Http::HeaderEntry& header) -> Http::HeaderMap::Iterate {
    std::cout << header.key().getStringView() << ": " << header.value().getStringView()
              << std::endl;
    return Http::HeaderMap::Iterate::Continue;
  });

  EXPECT_EQ(response_headers.get(LowerCaseString("new_key"))[0]->value().getStringView(), "value");
  EXPECT_EQ(response_headers.get(LowerCaseString("existing_key"))[0]->value().getStringView(),
            "new_value");
  EXPECT_EQ(response_headers.get(LowerCaseString("existing_key_with_multiple_values")).size(), 1);
  EXPECT_EQ(response_headers.get(LowerCaseString("existing_key_with_multiple_values"))[0]
                ->value()
                .getStringView(),
            "unique_value");
  EXPECT_FALSE(response_headers.has(LowerCaseString("to_delete")));
}

TEST(TestABI, GetRequestBodyLength) {
  Buffer::OwnedImpl body;
  body.add("hello");
  body.add("world");
  EXPECT_EQ(__envoy_dynamic_module_v1_http_get_request_body_buffer_length(&body), 10);
}

TEST(TestABI, GetResponseBodyLength) {
  Buffer::OwnedImpl body;
  body.add("a");
  body.add("b");
  EXPECT_EQ(__envoy_dynamic_module_v1_http_get_response_body_buffer_length(&body), 2);
}

TEST(TestABI, ResponseBodyAppendPrependDrain) {
  Buffer::OwnedImpl body;
  body.add("[INITIAL_VALUE]");
  const char* data = "hello";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr data_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(data);
  __envoy_dynamic_module_v1_type_InModuleBufferLength data_length = strlen(data);
  __envoy_dynamic_module_v1_http_get_response_body_buffer_append(&body, data_ptr, data_length);
  EXPECT_EQ(body.toString(), "[INITIAL_VALUE]hello");

  const char* data2 = "world";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr data_ptr2 =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(data2);
  __envoy_dynamic_module_v1_type_InModuleBufferLength data_length2 = strlen(data2);
  __envoy_dynamic_module_v1_http_get_response_body_buffer_prepend(&body, data_ptr2, data_length2);
  EXPECT_EQ(body.toString(), "world[INITIAL_VALUE]hello");

  __envoy_dynamic_module_v1_http_get_response_body_buffer_drain(&body, 5);
  EXPECT_EQ(body.toString(), "[INITIAL_VALUE]hello");

  // Drain the rest of the buffer.
  __envoy_dynamic_module_v1_http_get_response_body_buffer_drain(&body, 20);
  EXPECT_EQ(body.toString(), "");
}

TEST(TestABI, RequestBodyAppendPrependDrain) {
  Buffer::OwnedImpl body;
  body.add("[INITIAL_VALUE]");
  const char* data = "hello";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr data_ptr =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(data);
  __envoy_dynamic_module_v1_type_InModuleBufferLength data_length = strlen(data);
  __envoy_dynamic_module_v1_http_get_request_body_buffer_append(&body, data_ptr, data_length);
  EXPECT_EQ(body.toString(), "[INITIAL_VALUE]hello");

  const char* data2 = "world";
  __envoy_dynamic_module_v1_type_InModuleBufferPtr data_ptr2 =
      reinterpret_cast<__envoy_dynamic_module_v1_type_InModuleBufferPtr>(data2);
  __envoy_dynamic_module_v1_type_InModuleBufferLength data_length2 = strlen(data2);
  __envoy_dynamic_module_v1_http_get_request_body_buffer_prepend(&body, data_ptr2, data_length2);
  EXPECT_EQ(body.toString(), "world[INITIAL_VALUE]hello");

  __envoy_dynamic_module_v1_http_get_request_body_buffer_drain(&body, 5);
  EXPECT_EQ(body.toString(), "[INITIAL_VALUE]hello");

  // Drain the rest of the buffer.
  __envoy_dynamic_module_v1_http_get_request_body_buffer_drain(&body, 20);
  EXPECT_EQ(body.toString(), "");
}

TEST(TestABIRoundTrip, BodyManipulations) {
  DynamicModuleSharedPtr module = std::make_shared<DynamicModule>(
      "./test/test_programs/libmanipulate_body.so", "config", "TestABIRoundTripBodyManipulations");
  auto filter = std::make_shared<HttpFilter>(module);
  filter->ensureStreamContext();

  Buffer::OwnedImpl request_body;
  request_body.add("hello");
  request_body.add(" ");
  request_body.add("world");
  EXPECT_EQ(filter->decodeData(request_body, false), FilterDataStatus::Continue);
  EXPECT_EQ(request_body.toString(), "EEEEEEEEEEE");
  EXPECT_EQ(filter->decodeData(request_body, true), FilterDataStatus::Continue);
  EXPECT_EQ(request_body.toString(), "EnvoyEEEEE!");

  Buffer::OwnedImpl response_body;
  response_body.add("hello world");
  EXPECT_EQ(filter->encodeData(response_body, false), FilterDataStatus::Continue);
  EXPECT_EQ(response_body.toString(), "EEEEEEEEEEE");
  EXPECT_EQ(filter->encodeData(response_body, true), FilterDataStatus::Continue);
  EXPECT_EQ(response_body.toString(), "EnvoyEEEEE!");
}

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
