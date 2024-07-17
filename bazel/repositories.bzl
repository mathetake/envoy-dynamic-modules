load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("//bazel:envoy_version.bzl", "ENVOY_VERSION")

ENVOY_VERSIONS = {
    "1.30.4": {
        "sha256": "f334be38316bc38d93459dd3f2d1a356c5094d975438cdc0d92b7f36175bdfa9",
        "url": "https://github.com/envoyproxy/envoy/archive/v1.30.4.tar.gz",
        "strip_prefix": "envoy-1.30.4",
    },
}

def repositories():
    # check if the version is in the ENVOY_VERSIONS dict.
    if ENVOY_VERSION not in ENVOY_VERSIONS:
        fail("Unsupported Envoy version: %s" % ENVOY_VERSION)

    version = ENVOY_VERSIONS[ENVOY_VERSION]
    http_archive(
        name = "envoy",
        **version
    )
