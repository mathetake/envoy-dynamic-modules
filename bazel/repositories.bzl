load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

ENVOY_VERSION = "1.30.4"  # Change this to the desired Envoy version.

ENVOY_VERSIONS = {
    "1.30.4": {
        "sha256": "f334be38316bc38d93459dd3f2d1a356c5094d975438cdc0d92b7f36175bdfa9",
        "url": "https://github.com/envoyproxy/envoy/archive/v1.30.4.tar.gz",
        "strip_prefix": "envoy-1.30.4",
    },
    "1.29.7": {
        "sha256": "640f97362ccfe07eaf0ac83c0050d5348f49d10d05e22072091aef2af602e9ec",
        "url": "https://github.com/envoyproxy/envoy/archive/v1.29.7.tar.gz",
        "strip_prefix": "envoy-1.29.7",
    },
}

def repositories():
    # check if the version is in the ENVOY_VERSIONS dict.
    if ENVOY_VERSION not in ENVOY_VERSIONS:
        fail("Unsupported Envoy version: %s" % ENVOY_VERSION)

    http_archive(
        name = "envoy",
        **ENVOY_VERSIONS[ENVOY_VERSION]
    )

    http_archive(
        name = "envoyx-abi",
        url = "https://github.com/envoyproxyx/abi/archive/e96e8cec29a82da7717528afd7242cfdb69a4219.tar.gz",
        sha256 = "de8f0332e350cf530a228b16bc58aa5f9a038bed320b2250f03c6c16fa48eb39",
        strip_prefix = "abi-e96e8cec29a82da7717528afd7242cfdb69a4219",
        build_file_content = """
cc_library(
    name = "abi_lib",
    hdrs = ["abi.h"],
    visibility = ["//visibility:public"],
)
""",
    )
