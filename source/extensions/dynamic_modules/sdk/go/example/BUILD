load("@io_bazel_rules_go//go:def.bzl", "go_binary", "go_test")

go_binary(
    name = "example.so",
    srcs = glob(["*.go"]),
    out = "libexample.so",
    cgo = True,
    importpath = "github.com/mathetake/envoy-dynamic-modules/source/extensions/dynmic_modules/sdk/go/example",
    linkmode = "c-shared",
    visibility = ["//visibility:public"],
    deps = [
        "//source/extensions/dynamic_modules/sdk/go/envoy:envoy_dynamic_modules_go_sdk",
    ],
)

go_test(
    name = "example_test",
    srcs = glob(["*.go"]),
    cgo = False,
    importpath = "github.com/mathetake/envoy-dynamic-modules/source/extensions/dynmic_modules/sdk/go/example",
    pure = "on",
    deps = [
        "//source/extensions/dynamic_modules/sdk/go/envoy:envoy_dynamic_modules_go_sdk",
    ],
)
