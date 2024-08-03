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
    name = "http_dynamic_module_lib",
    srcs = ["http_dynamic_module.cc"],
    hdrs = [
        "http_dynamic_module.h",
        "//source/extensions/dynamic_modules/abi:abi.h",
    ],
    copts = COPTS,
    repository = "@envoy",
    deps = [
        ":pkg_cc_proto",
        "//source/extensions/dynamic_modules:dynamic_modules_lib",
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
        ":http_dynamic_module_lib",
        ":pkg_cc_proto",
        "@envoy//source/extensions/filters/http/common:pass_through_filter_lib",
    ],
)

envoy_cc_library(
    name = "abi_lib",
    srcs = [
        "abi.cc",
        "//source/extensions/dynamic_modules/abi:abi.h",
    ],
    copts = COPTS,
    # Make sure that the symbols are exported so that they can be found by the dynamic loader.
    linkopts = select({
        "@bazel_tools//src/conditions:darwin": ["-rdynamic"],
        "//conditions:default": ["-Wl,--export-dynamic"],
    }),
    repository = "@envoy",
    deps = [
        ":filter_lib",
    ],
)

envoy_cc_library(
    name = "factory",
    srcs = [
        "factory.cc",
    ],
    copts = COPTS,
    repository = "@envoy",
    deps = [
        ":abi_lib",
        ":filter_lib",
    ],
    alwayslink = True,
)
