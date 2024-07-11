load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_binary",
)

package(default_visibility = ["//visibility:public"])

envoy_cc_binary(
    name = "envoy",
    repository = "@envoy",
    deps = [
        "//x:factory",
        "@envoy//source/exe:envoy_main_entry_lib",
    ],
)
