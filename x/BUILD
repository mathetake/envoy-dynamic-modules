load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
)
load("@envoy_api//bazel:api_build_system.bzl", "api_proto_package")

package(default_visibility = ["//visibility:public"])

api_proto_package(
    deps = [
        "@envoy_api//envoy/config/core/v3:pkg",
    ],
)

COPTS = ["-DENVOY_DYNAMIC_MODULE=1"]

envoy_cc_library(
    name = "dynamic_module_lib",
    srcs = ["dynamic_module.cc"],
    hdrs = [
        "abi.h",
        "dynamic_module.h",
    ],
    copts = COPTS,
    repository = "@envoy",
    deps = [
        ":pkg_cc_proto",
        "@envoy//envoy/common:exception_lib",
        "@envoy//envoy/server:filter_config_interface",
        "@envoy//source/extensions/filters/http/common:pass_through_filter_lib",
    ],
)

envoy_cc_library(
    name = "filter_lib",
    srcs = ["filter.cc"],
    hdrs = ["filter.h"],
    copts = COPTS,
    repository = "@envoy",
    deps = [
        ":dynamic_module_lib",
        ":pkg_cc_proto",
        "@envoy//source/extensions/filters/http/common:pass_through_filter_lib",
    ],
)

envoy_cc_library(
    name = "abi_lib",
    srcs = ["abi.cc"],
    copts = COPTS,
    # Make sure that the symbols are exported so that they can be found by the dynamic loader.
    linkopts = ["-Wl,--export-dynamic"],
    repository = "@envoy",
    deps = [
        ":filter_lib",
    ],
)

envoy_cc_library(
    name = "factory",
    srcs = [
        "abi.h",
        "factory.cc",
    ],
    copts = COPTS,
    repository = "@envoy",
    deps = [
        ":filter_lib",
    ],
    alwayslink = True,
)
