#include "gtest/gtest.h"
#include <memory>

#include "x/dynamic_module.h"
#include "x/filter.h"

#include "test/test_common/utility.h"
#include "test/test_util.h"

namespace Envoy {
namespace Http {
namespace DynamicModule {

TEST(TestHttpFilter, StreamContextNull) {
  DynamicModuleSharedPtr module = loadTestDynamicModule("init", "StreamContextNull", "config");
  auto filter = std::make_shared<HttpFilter>(module);
  Http::TestRequestHeaderMapImpl request_headers{};
  const auto result = filter->decodeHeaders(request_headers, false);
  EXPECT_EQ(result, FilterHeadersStatus::StopIteration);
}

TEST(TestHttpFilter, StreamContextOK) {
  DynamicModuleSharedPtr module = loadTestDynamicModule("stream_init", "StreamContextOK", "");
  auto filter = std::make_shared<HttpFilter>(module);
  Http::TestRequestHeaderMapImpl request_headers{};
  const auto result = filter->decodeHeaders(request_headers, false);
  EXPECT_EQ(result, FilterHeadersStatus::Continue);

  const void* context = filter->streamContextForTesting();
  EXPECT_NE(context, nullptr);
  // Read the integer value from the context which is set in the
  // __envoy_dynamic_module_v1_event_http_context_init function.
  const int* value = static_cast<const int*>(context);
  EXPECT_EQ(*value, 999999);
}

} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
