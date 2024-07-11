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

envoy_cc_library(
    name = "dynamic_module_lib",
    srcs = ["dynamic_module.cc"],
    hdrs = ["dynamic_module.h"],
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
    repository = "@envoy",
    deps = [
        ":dynamic_module_lib",
        ":pkg_cc_proto",
        "@envoy//source/extensions/filters/http/common:pass_through_filter_lib",
    ],
)

envoy_cc_library(
    name = "factory",
    srcs = ["factory.cc"],
    repository = "@envoy",
    deps = [
        ":filter_lib",
    ],
)
