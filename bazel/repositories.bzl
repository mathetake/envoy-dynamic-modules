load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

ENVOY_VERSION = "1.31.0"  # Change this to the desired Envoy version.

ENVOY_VERSIONS = {
    "1.31.0": {
        "sha256": "39ba37aed81a9d4988a5736cf558243179f2bf1490843da25687d1aafd9d01c6",
        "url": "https://github.com/envoyproxy/envoy/archive/v1.31.0.tar.gz",
        "strip_prefix": "envoy-1.31.0",
    },
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

    #     http_archive(
    #         name = "envoyx-abi",
    #         url = "https://github.com/envoyproxyx/abi/archive/511df259b121a0aae92c21311cafbf4cc311c9e9.tar.gz",
    #         sha256 = "a53a44ebc1ec7b64fdbf88de5485fee1a82be906deb31e5bb04da9d4e4657a7c",
    #         strip_prefix = "abi-511df259b121a0aae92c21311cafbf4cc311c9e9",
    #         build_file_content = """
    # cc_library(
    #     name = "abi_lib",
    #     hdrs = ["abi.h"],
    #     visibility = ["//visibility:public"],
    # )
    # """,
    #     )

    native.new_local_repository(
        name = "envoyx-abi",
        path = "/home/mathetake/envoyxabi",
        build_file_content = """
exports_files(["abi.h"])

cc_library(
    name = "abi_lib",
    hdrs = ["abi.h"],
    visibility = ["//visibility:public"],
)
""",
    )
