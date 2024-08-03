load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
)

package(default_visibility = ["//visibility:public"])

envoy_cc_library(
    name = "dynamic_modules_lib",
    srcs = [
        "dynamic_modules.cc",
    ],
    hdrs = [
        "dynamic_modules.h",
        "//source/extensions/dynamic_modules/abi:abi.h",
    ],
    repository = "@envoy",
    deps = [
        "@envoy//envoy/common:exception_lib",
    ],
)
