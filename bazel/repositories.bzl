load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def repositories():
    http_archive(
        name = "envoy",
        sha256 = "f334be38316bc38d93459dd3f2d1a356c5094d975438cdc0d92b7f36175bdfa9",
        strip_prefix = "envoy-1.30.4",
        url = "https://github.com/envoyproxy/envoy/archive/v1.30.4.tar.gz",
    )
