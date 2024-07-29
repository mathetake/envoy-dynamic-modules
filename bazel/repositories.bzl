load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def repositories():
    http_archive(
        name = "envoy",
        sha256 = "39ba37aed81a9d4988a5736cf558243179f2bf1490843da25687d1aafd9d01c6",
        strip_prefix = "envoy-1.31.0",
        url = "https://github.com/envoyproxy/envoy/archive/v1.31.0.tar.gz",
    )
