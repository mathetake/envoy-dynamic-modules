#include "gtest/gtest.h"
#include <memory>

#include "source/extensions/dynamic_modules/abi/abi.h"
#include "source/extensions/dynamic_modules/http/filter.h"

#include "test/test_common/utility.h"
#include "test/extensions/dynamic_modules/http/test_util.h"

namespace Envoy {
namespace Extensions {
namespace DynamicModules {
namespace Http {

TEST(TestABI, GetRequestHeaderValueEmpty) {
  Http::TestRequestHeaderMapImpl request_headers{};
  envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers = &request_headers;
  std::string key = "key";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key.data());
  envoy_dynamic_module_type_InModuleBufferLength key_length = key.size();
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length_ptr;
  int num_values = envoy_dynamic_module_http_get_request_header_value(
      headers, key_ptr, key_length,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)(envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length_ptr);

  EXPECT_EQ(num_values, 0);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length_ptr, 0);
}

TEST(TestABI, GetResponseHeaderValueEmpty) {
  Http::TestResponseHeaderMapImpl response_headers{};
  envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers = &response_headers;
  std::string key = "key";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key.data());
  envoy_dynamic_module_type_InModuleBufferLength key_length = key.size();
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length_ptr;
  int num_values = envoy_dynamic_module_http_get_response_header_value(
      headers, key_ptr, key_length,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length_ptr);
  EXPECT_EQ(num_values, 0);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length_ptr, 0);
}

TEST(TestABI, GetRequestHeaderValue) {
  Http::TestRequestHeaderMapImpl request_headers{{":path", "/"}, {"key", "value"}};
  envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers = &request_headers;

  std::string key = "key";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key.data());
  envoy_dynamic_module_type_InModuleBufferLength key_length = key.size();
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length_ptr;
  int num_values = envoy_dynamic_module_http_get_request_header_value(
      headers, key_ptr, key_length,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length_ptr);
  EXPECT_EQ(num_values, 1);
  // Read the value from the buffer.
  std::string result = std::string(static_cast<char*>(result_buffer_ptr), result_buffer_length_ptr);
  EXPECT_EQ(result, "value");

  // Test different key.
  std::string key2 = ":path";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr2 =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key2.data());
  envoy_dynamic_module_type_InModuleBufferLength key_length2 = key2.size();
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr2;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length_ptr2;
  int num_values2 = envoy_dynamic_module_http_get_request_header_value(
      headers, key_ptr2, key_length2, &result_buffer_ptr2,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length_ptr2);
  EXPECT_EQ(num_values2, 1);
  // Read the value from the buffer.
  std::string result2 =
      std::string(static_cast<char*>(result_buffer_ptr2), result_buffer_length_ptr2);
  EXPECT_EQ(result2, "/");
}

TEST(TestABI, GetResponseHeaderValue) {
  Http::TestResponseHeaderMapImpl response_headers{{"foo", "bar"}};
  envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers = &response_headers;
  std::string key = "foo";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key.data());
  envoy_dynamic_module_type_InModuleBufferLength key_length = key.size();
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length_ptr;
  int num_values = envoy_dynamic_module_http_get_response_header_value(
      headers, key_ptr, key_length,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length_ptr);
  EXPECT_EQ(num_values, 1);
  // Read the value from the buffer.
  std::string result = std::string(static_cast<char*>(result_buffer_ptr), result_buffer_length_ptr);
  EXPECT_EQ(result, "bar");

  // Test different key.
  std::string key2 = "non-existent-key";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr2 =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key2.data());
  envoy_dynamic_module_type_InModuleBufferLength key_length2 = key2.size();
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr2;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length_ptr2;
  int num_values2 = envoy_dynamic_module_http_get_response_header_value(
      headers, key_ptr2, key_length2, &result_buffer_ptr2,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length_ptr2);
  EXPECT_EQ(num_values2, 0);
  EXPECT_EQ(result_buffer_ptr2, nullptr);
  EXPECT_EQ(result_buffer_length_ptr2, 0);
}

TEST(TestABI, GetRequestHeaderValueNth) {
  Http::TestRequestHeaderMapImpl request_headers{};
  request_headers.addCopy(LowerCaseString("key"), "value1");
  request_headers.addCopy(LowerCaseString("key"), "value2");
  envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers = &request_headers;
  std::string key = "key";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key.data());
  envoy_dynamic_module_type_InModuleBufferLength key_length = key.size();
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length_ptr;
  envoy_dynamic_module_http_get_request_header_value_nth(
      headers, key_ptr, key_length,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length_ptr, 1);
  // Read the value from the buffer.
  std::string result = std::string(static_cast<char*>(result_buffer_ptr), result_buffer_length_ptr);
  EXPECT_EQ(result, "value2");
}

TEST(TestABI, GetResponseHeaderValueNth) {
  Http::TestResponseHeaderMapImpl response_headers{};
  response_headers.addCopy(LowerCaseString("key"), "value1");
  response_headers.addCopy(LowerCaseString("key"), "value2");
  envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers = &response_headers;
  std::string key = "key";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key.data());
  envoy_dynamic_module_type_InModuleBufferLength key_length = key.size();
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length_ptr;
  envoy_dynamic_module_http_get_response_header_value_nth(
      headers, key_ptr, key_length,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length_ptr, 1);
  // Read the value from the buffer.
  std::string result = std::string(static_cast<char*>(result_buffer_ptr), result_buffer_length_ptr);
  EXPECT_EQ(result, "value2");
}

TEST(TestABI, GetRequestHeaderValueNthOutOfBounds) {
  Http::TestRequestHeaderMapImpl request_headers{};
  request_headers.addCopy(LowerCaseString("key"), "value1");
  envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers = &request_headers;
  std::string key = "key";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key.data());
  envoy_dynamic_module_type_InModuleBufferLength key_length = key.size();
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length_ptr;
  envoy_dynamic_module_http_get_request_header_value_nth(
      headers, key_ptr, key_length,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length_ptr, 1);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length_ptr, 0);
}

TEST(TestABI, GetResponseHeaderValueNthOutOfBounds) {
  Http::TestResponseHeaderMapImpl response_headers{};
  response_headers.addCopy(LowerCaseString("key"), "value1");
  envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers = &response_headers;
  std::string key = "key";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key.data());
  envoy_dynamic_module_type_InModuleBufferLength key_length = key.size();
  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length_ptr;
  envoy_dynamic_module_http_get_response_header_value_nth(
      headers, key_ptr, key_length,
      (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length_ptr, 1);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length_ptr, 0);
}

TEST(TestABIRoundTrip, GetHeaders) {
  HttpDynamicModuleSharedPtr module = loadTestDynamicModule("get_headers", "config");
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
  EXPECT_EQ(envoy_dynamic_module_http_get_request_body_buffer_slices_count(&body), 0);

  body.appendSliceForTest("hello");
  EXPECT_EQ(envoy_dynamic_module_http_get_request_body_buffer_slices_count(&body), 1);

  body.appendSliceForTest("world");
  EXPECT_EQ(envoy_dynamic_module_http_get_request_body_buffer_slices_count(&body), 2);
}

TEST(TestABI, GetResponseBodyBufferSlicesCount) {
  Buffer::OwnedImpl body;
  EXPECT_EQ(envoy_dynamic_module_http_get_response_body_buffer_slices_count(&body), 0);

  body.appendSliceForTest("hello");
  EXPECT_EQ(envoy_dynamic_module_http_get_response_body_buffer_slices_count(&body), 1);

  body.appendSliceForTest("world");
  EXPECT_EQ(envoy_dynamic_module_http_get_response_body_buffer_slices_count(&body), 2);
}

TEST(TestABI, GetRequestBodyBufferSlice) {
  Buffer::OwnedImpl body;
  body.appendSliceForTest("hello");
  body.appendSliceForTest("world");

  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length;
  envoy_dynamic_module_http_get_request_body_buffer_slice(
      &body, 0, (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);

  std::string result = std::string(static_cast<char*>(result_buffer_ptr), result_buffer_length);
  EXPECT_EQ(result, "hello");

  envoy_dynamic_module_http_get_request_body_buffer_slice(
      &body, 1, (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);
  std::string result2 = std::string(static_cast<char*>(result_buffer_ptr), result_buffer_length);
  EXPECT_EQ(result2, "world");

  envoy_dynamic_module_http_get_request_body_buffer_slice(
      &body, 2, (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);
  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length, 0);
}

TEST(TestABI, GetResponseBodyBufferSlice) {
  Buffer::OwnedImpl body;
  body.appendSliceForTest("hello");
  body.appendSliceForTest("world");

  envoy_dynamic_module_type_InModuleBufferPtr result_buffer_ptr;
  envoy_dynamic_module_type_InModuleBufferLength result_buffer_length;
  envoy_dynamic_module_http_get_response_body_buffer_slice(
      &body, 0, (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);
  std::string result = std::string(static_cast<char*>(result_buffer_ptr), result_buffer_length);
  EXPECT_EQ(result, "hello");

  envoy_dynamic_module_http_get_response_body_buffer_slice(
      &body, 1, (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);
  std::string result2 = std::string(static_cast<char*>(result_buffer_ptr), result_buffer_length);
  EXPECT_EQ(result2, "world");

  envoy_dynamic_module_http_get_response_body_buffer_slice(
      &body, 2, (envoy_dynamic_module_type_DataSlicePtrResult)&result_buffer_ptr,
      (envoy_dynamic_module_type_DataSliceLengthResult)&result_buffer_length);

  EXPECT_EQ(result_buffer_ptr, nullptr);
  EXPECT_EQ(result_buffer_length, 0);
}

TEST(TestABIRoundTrip, GetBody) {
  HttpDynamicModuleSharedPtr module = loadTestDynamicModule("get_body", "config");
  auto filter = std::make_shared<HttpFilter>(module);
  filter->ensureHttpFilterInstance();

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
  envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers = &request_headers;
  char key[] = "key";
  char value[] = "value";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key);
  envoy_dynamic_module_type_InModuleBufferLength key_length = strlen(key);
  envoy_dynamic_module_type_InModuleBufferPtr value_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(value);
  envoy_dynamic_module_type_InModuleBufferLength value_length = strlen(value);
  envoy_dynamic_module_http_set_request_header(headers, key_ptr, key_length, value_ptr,
                                               value_length);
  EXPECT_EQ(request_headers.get(LowerCaseString(key))[0]->value().getStringView(), value);
}

TEST(TestABI, SetHeader_Add_empty_value) {
  Http::TestRequestHeaderMapImpl request_headers{};
  envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers = &request_headers;
  char key[] = "key";
  char value[] = "";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key);
  envoy_dynamic_module_type_InModuleBufferLength key_length = strlen(key);
  envoy_dynamic_module_type_InModuleBufferPtr value_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(value);
  envoy_dynamic_module_type_InModuleBufferLength value_length = strlen(value);
  envoy_dynamic_module_http_set_request_header(headers, key_ptr, key_length, value_ptr,
                                               value_length);
  EXPECT_EQ(request_headers.get(LowerCaseString(key))[0]->value().getStringView(), value);
}

TEST(TestABI, SetHeader_Remove) {
  Http::TestRequestHeaderMapImpl request_headers{{"key", "value"}};
  envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers = &request_headers;
  char key[] = "key";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key);
  envoy_dynamic_module_type_InModuleBufferLength key_length = strlen(key);
  envoy_dynamic_module_type_InModuleBufferPtr value_ptr = nullptr;
  envoy_dynamic_module_type_InModuleBufferLength value_length = 0;
  envoy_dynamic_module_http_set_request_header(headers, key_ptr, key_length, value_ptr,
                                               value_length);
  EXPECT_EQ(request_headers.size(), 0);
}

TEST(TestABI, SetHeader_Replaces) {
  Http::TestRequestHeaderMapImpl request_headers{{"key", "value"}};
  envoy_dynamic_module_type_HttpResponseHeaderMapPtr headers = &request_headers;
  char key[] = "key";
  char value[] = "new_value";
  envoy_dynamic_module_type_InModuleBufferPtr key_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(key);
  envoy_dynamic_module_type_InModuleBufferLength key_length = strlen(key);
  envoy_dynamic_module_type_InModuleBufferPtr value_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(value);
  envoy_dynamic_module_type_InModuleBufferLength value_length = strlen(value);
  envoy_dynamic_module_http_set_request_header(headers, key_ptr, key_length, value_ptr,
                                               value_length);
  EXPECT_EQ(request_headers.get(LowerCaseString(key))[0]->value().getStringView(), value);
}

TEST(TestABIRoundTrip, SetHeaders) {
  HttpDynamicModuleSharedPtr module = loadTestDynamicModule("set_headers", "config");
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
  EXPECT_EQ(envoy_dynamic_module_http_get_request_body_buffer_length(&body), 10);
}

TEST(TestABI, GetResponseBodyLength) {
  Buffer::OwnedImpl body;
  body.add("a");
  body.add("b");
  EXPECT_EQ(envoy_dynamic_module_http_get_response_body_buffer_length(&body), 2);
}

TEST(TestABI, ResponseBodyAppendPrependDrain) {
  Buffer::OwnedImpl body;
  body.add("[INITIAL_VALUE]");
  std::string data = "hello";
  envoy_dynamic_module_type_InModuleBufferPtr data_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(data.data());
  envoy_dynamic_module_type_InModuleBufferLength data_length = data.size();
  envoy_dynamic_module_http_append_response_body_buffer(&body, data_ptr, data_length);
  EXPECT_EQ(body.toString(), "[INITIAL_VALUE]hello");

  std::string data2 = "world";
  envoy_dynamic_module_type_InModuleBufferPtr data_ptr2 =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(data2.data());
  envoy_dynamic_module_type_InModuleBufferLength data_length2 = data2.size();
  envoy_dynamic_module_http_prepend_response_body_buffer(&body, data_ptr2, data_length2);
  EXPECT_EQ(body.toString(), "world[INITIAL_VALUE]hello");

  envoy_dynamic_module_http_drain_response_body_buffer(&body, 5);
  EXPECT_EQ(body.toString(), "[INITIAL_VALUE]hello");

  // Drain the rest of the buffer.
  envoy_dynamic_module_http_drain_response_body_buffer(&body, 20);
  EXPECT_EQ(body.toString(), "");
}

TEST(TestABI, RequestBodyAppendPrependDrain) {
  Buffer::OwnedImpl body;
  body.add("[INITIAL_VALUE]");
  std::string data = "hello";
  envoy_dynamic_module_type_InModuleBufferPtr data_ptr =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(data.data());
  envoy_dynamic_module_type_InModuleBufferLength data_length = data.size();
  envoy_dynamic_module_http_append_request_body_buffer(&body, data_ptr, data_length);
  EXPECT_EQ(body.toString(), "[INITIAL_VALUE]hello");

  std::string data2 = "world";
  envoy_dynamic_module_type_InModuleBufferPtr data_ptr2 =
      reinterpret_cast<envoy_dynamic_module_type_InModuleBufferPtr>(data2.data());
  envoy_dynamic_module_type_InModuleBufferLength data_length2 = data2.size();
  envoy_dynamic_module_http_prepend_request_body_buffer(&body, data_ptr2, data_length2);
  EXPECT_EQ(body.toString(), "world[INITIAL_VALUE]hello");

  envoy_dynamic_module_http_drain_request_body_buffer(&body, 5);
  EXPECT_EQ(body.toString(), "[INITIAL_VALUE]hello");

  // Drain the rest of the buffer.
  envoy_dynamic_module_http_drain_request_body_buffer(&body, 20);
  EXPECT_EQ(body.toString(), "");
}

TEST(TestABIRoundTrip, BodyManipulations) {
  HttpDynamicModuleSharedPtr module = loadTestDynamicModule("manipulate_body", "config");
  auto filter = std::make_shared<HttpFilter>(module);
  filter->ensureHttpFilterInstance();

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

TEST(TestABI, BufferCopyOut) {
  Buffer::OwnedImpl buffer;
  buffer.add("hello");
  buffer.add("world");
  {
    std::vector<char> result_buffer(10);
    envoy_dynamic_module_http_copy_out_request_body_buffer(&buffer, 0, 10, result_buffer.data());
    EXPECT_EQ(std::string(result_buffer.data(), result_buffer.size()), "helloworld");
  }
  {
    std::vector<char> result_buffer(1000);
    envoy_dynamic_module_http_copy_out_request_body_buffer(&buffer, 5, 5, result_buffer.data());
    EXPECT_EQ(std::string(result_buffer.data(), 5), "world");
  }
  {
    std::vector<char> result_buffer(10);
    envoy_dynamic_module_http_copy_out_response_body_buffer(&buffer, 0, 10, result_buffer.data());
    EXPECT_EQ(std::string(result_buffer.data(), result_buffer.size()), "helloworld");
  }
  {
    std::vector<char> result_buffer(1000);
    envoy_dynamic_module_http_copy_out_response_body_buffer(&buffer, 5, 5, result_buffer.data());
    EXPECT_EQ(std::string(result_buffer.data(), 5), "world");
  }
}

} // namespace Http
} // namespace DynamicModules
} // namespace Extensions
} // namespace Envoy
