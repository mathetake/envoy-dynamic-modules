load("@io_bazel_rules_go//go:def.bzl", "go_library", "go_test")

go_library(
    name = "envoy_dynamic_modules_go_sdk",
    srcs = glob(["*.go"]) + ["abi.h"],
    cgo = True,
    importpath = "github.com/mathetake/envoy-dynamic-modules/sdks/go/envoy",
    visibility = ["//visibility:public"],
)

go_test(
    name = "envoy_dynamic_modules_go_sdk_test",
    srcs = glob(["*.go"]),
    cgo = False,
    importpath = "github.com/mathetake/envoy-dynamic-modules/sdks/go/envoy",
    pure = "on",
)
