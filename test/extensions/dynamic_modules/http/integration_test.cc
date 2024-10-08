#include "test/integration/http_integration.h"

namespace Envoy {
class HttpFilterIntegrationTest : public HttpIntegrationTest,
                                  public testing::TestWithParam<Network::Address::IpVersion> {
public:
  HttpFilterIntegrationTest() : HttpIntegrationTest(Http::CodecClient::Type::HTTP1, GetParam()) {}

  void initializeDynamicFilter(const std::string& filter_name, const std::string& config = "") {
    constexpr auto format = R"EOF(
name: envoy.http.dynamic_modules
typed_config:
  "@type": type.googleapis.com/envoy.extensions.filters.http.dynamic_modules.v3.DynamicModuleConfig
  file_path: {}
  filter_config: "{}"
)EOF";
    config_helper_.prependFilter(fmt::format(format, filter_name, config));
    initialize();
  }
};

INSTANTIATE_TEST_SUITE_P(IpVersions, HttpFilterIntegrationTest,
                         testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(HttpFilterIntegrationTest, Headers) {
  initializeDynamicFilter(
      "./test/extensions/dynamic_modules/http/test_programs/libintegration_test_headers.so");

  Http::TestRequestHeaderMapImpl headers{
      {":method", "GET"}, {":path", "/"}, {":authority", "host"}};
  Http::TestRequestHeaderMapImpl response_headers{{":status", "404"}};

  IntegrationCodecClientPtr codec_client;
  FakeHttpConnectionPtr fake_upstream_connection;
  FakeStreamPtr request_stream;

  codec_client = makeHttpConnection(lookupPort("http"));
  auto response = codec_client->makeHeaderOnlyRequest(headers);
  ASSERT_TRUE(fake_upstreams_[0]->waitForHttpConnection(*dispatcher_, fake_upstream_connection));
  ASSERT_TRUE(fake_upstream_connection->waitForNewStream(*dispatcher_, request_stream));
  ASSERT_TRUE(request_stream->waitForEndStream(*dispatcher_));
  request_stream->encodeHeaders(response_headers, true);
  ASSERT_TRUE(response->waitForEndStream());

  auto new_key_values = response->headers().get(Http::LowerCaseString("new_key"));
  EXPECT_EQ("value", new_key_values.empty() ? "" : new_key_values[0]->value().getStringView());
  EXPECT_EQ("200", response->headers().getStatusValue());

  codec_client->close();
}

TEST_P(HttpFilterIntegrationTest, HeadersStopIteration) {
  initializeDynamicFilter(
      "./test/extensions/dynamic_modules/http/test_programs/libintegration_test_headers.so",
      "should_wait");

  Http::TestRequestHeaderMapImpl headers{
      {":method", "GET"}, {":path", "/"}, {":authority", "host"}};
  Http::TestRequestHeaderMapImpl response_headers{{":status", "404"}};

  IntegrationCodecClientPtr codec_client;
  FakeHttpConnectionPtr fake_upstream_connection;
  FakeStreamPtr request_stream;

  codec_client = makeHttpConnection(lookupPort("http"));
  auto response = codec_client->makeHeaderOnlyRequest(headers);
  ASSERT_TRUE(fake_upstreams_[0]->waitForHttpConnection(*dispatcher_, fake_upstream_connection));
  ASSERT_TRUE(fake_upstream_connection->waitForNewStream(*dispatcher_, request_stream));
  ASSERT_TRUE(request_stream->waitForEndStream(*dispatcher_));
  request_stream->encodeHeaders(response_headers, false);
  request_stream->encodeData(0, true);
  ASSERT_TRUE(response->waitForEndStream(std::chrono::milliseconds(3000)));
  EXPECT_EQ("404", response->headers().getStatusValue());

  codec_client->close();
}

TEST_P(HttpFilterIntegrationTest, Bodies) {
  initializeDynamicFilter(
      "./test/extensions/dynamic_modules/http/test_programs/libintegration_test_bodies.so", "");

  Http::TestRequestHeaderMapImpl headers{
      {":method", "GET"}, {":path", "/"}, {":authority", "host"}};
  Http::TestRequestHeaderMapImpl response_headers{{":status", "200"}};

  IntegrationCodecClientPtr codec_client;
  FakeHttpConnectionPtr fake_upstream_connection;
  FakeStreamPtr request_stream;

  codec_client = makeHttpConnection(lookupPort("http"));
  auto response = codec_client->makeRequestWithBody(headers, "aloha", true);
  ASSERT_TRUE(fake_upstreams_[0]->waitForHttpConnection(*dispatcher_, fake_upstream_connection));
  ASSERT_TRUE(fake_upstream_connection->waitForNewStream(*dispatcher_, request_stream));
  ASSERT_TRUE(request_stream->waitForEndStream(*dispatcher_));
  ASSERT_TRUE(request_stream->waitForData(*dispatcher_, "AAAAA"));
  request_stream->encodeHeaders(response_headers, false);
  request_stream->encodeData("from", false);
  request_stream->encodeData("upstream", true);
  ASSERT_TRUE(response->waitForEndStream(std::chrono::milliseconds(3000)));
  EXPECT_EQ("BBBBBBBBBBBB", response->body());

  EXPECT_EQ("200", response->headers().getStatusValue());
  codec_client->close();
}

TEST_P(HttpFilterIntegrationTest, LocalResponse_OnRequest) {
  initializeDynamicFilter(
      "./test/extensions/dynamic_modules/http/test_programs/libintegration_test_local_response.so",
      "on_request");

  Http::TestRequestHeaderMapImpl headers{
      {":method", "GET"}, {":path", "/"}, {":authority", "host"}};
  Http::TestRequestHeaderMapImpl response_headers{{":status", "200"}};

  IntegrationCodecClientPtr codec_client;
  FakeHttpConnectionPtr fake_upstream_connection;
  FakeStreamPtr request_stream;

  codec_client = makeHttpConnection(lookupPort("http"));
  auto response = codec_client->makeRequestWithBody(headers, "aloha", true);
  ASSERT_TRUE(response->waitForEndStream(std::chrono::milliseconds(3000)));

  auto foo = response->headers().get(Http::LowerCaseString("foo"));
  EXPECT_EQ("bar", foo.empty() ? "" : foo[0]->value().getStringView());
  EXPECT_EQ("500", response->headers().getStatusValue());
  codec_client->close();
}

TEST_P(HttpFilterIntegrationTest, LocalResponse_OnRespnose) {
  initializeDynamicFilter(
      "./test/extensions/dynamic_modules/http/test_programs/libintegration_test_local_response.so",
      "on_response");

  Http::TestRequestHeaderMapImpl headers{
      {":method", "GET"}, {":path", "/"}, {":authority", "host"}};
  Http::TestRequestHeaderMapImpl response_headers{{":status", "200"}};

  IntegrationCodecClientPtr codec_client;
  FakeHttpConnectionPtr fake_upstream_connection;
  FakeStreamPtr request_stream;

  codec_client = makeHttpConnection(lookupPort("http"));
  auto response = codec_client->makeRequestWithBody(headers, "aloha", true);
  ASSERT_TRUE(fake_upstreams_[0]->waitForHttpConnection(*dispatcher_, fake_upstream_connection));
  ASSERT_TRUE(fake_upstream_connection->waitForNewStream(*dispatcher_, request_stream));
  request_stream->encodeHeaders(response_headers, false);
  ASSERT_TRUE(response->waitForEndStream(std::chrono::milliseconds(3000)));

  auto cat = response->headers().get(Http::LowerCaseString("cat"));
  EXPECT_EQ("dog", cat.empty() ? "" : cat[0]->value().getStringView());
  EXPECT_EQ("504", response->headers().getStatusValue());
  codec_client->close();
}

} // namespace Envoy
