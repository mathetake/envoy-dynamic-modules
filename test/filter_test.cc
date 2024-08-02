#include "gtest/gtest.h"
#include <memory>

#include "source/dynamic_module.h"
#include "source/filter.h"

#include "test/test_common/utility.h"
#include "test/test_util.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

TEST(TestHttpFilter, StreamContextNull) {
  DynamicModuleSharedPtr module = loadTestDynamicModule("init", "config");
  auto filter = std::make_shared<HttpFilter>(module);
  Http::TestRequestHeaderMapImpl request_headers{};
  const auto result = filter->decodeHeaders(request_headers, false);
  EXPECT_EQ(result, FilterHeadersStatus::StopIteration);
}

TEST(TestHttpFilter, StreamContextOK) {
  DynamicModuleSharedPtr module = loadTestDynamicModule("stream_init", "");
  auto filter = std::make_shared<HttpFilter>(module);
  Http::TestRequestHeaderMapImpl request_headers{};
  const auto result = filter->decodeHeaders(request_headers, false);
  EXPECT_EQ(result, FilterHeadersStatus::Continue);

  const void* instance = filter->http_filter_instance_;
  EXPECT_NE(instance, nullptr);
  // Read the integer value from the instance which is set in the
  // envoy_dynamic_module_event_http_filter_instance_init function.
  const int* value = static_cast<const int*>(instance);
  EXPECT_EQ(*value, 999999);
}

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
