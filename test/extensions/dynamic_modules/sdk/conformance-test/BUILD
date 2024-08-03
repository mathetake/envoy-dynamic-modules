load("@io_bazel_rules_go//go:def.bzl", "go_test")

go_test(
    name = "go_sdk_example_test",
    srcs = glob(["*.go"]),
    data = [
        "//:envoy",
        "//source/extensions/dynamic_modules/sdk/go/example:example.so",
    ],
    embedsrcs = ["envoy.yaml"],
    env = {
        "SHARED_LIBRARY_PATH": "envoyx/source/extensions/dynamic_modules/sdk/go/example/libexample.so",
        "ENVOY_PATH": "envoyx/envoy",
    },
    importpath = "github.com/mathetake/envoy-dynamic-modules/test/extensions/dynamic_modules/sdk/conformance-test",
    tags = ["exclusive"],
    deps = [
        "//source/extensions/dynamic_modules/sdk/go/envoy:envoy_dynamic_modules_go_sdk",
        "@com_github_testify//require",
    ],
)

go_test(
    name = "rust_sdk_example_test",
    srcs = glob(["*.go"]),
    data = [
        "//:envoy",
        "//source/extensions/dynamic_modules/sdk/rust:example",
    ],
    embedsrcs = ["envoy.yaml"],
    env = {
        "SHARED_LIBRARY_PATH": "envoyx/source/extensions/dynamic_modules/sdk/rust/libexample.so",
        "ENVOY_PATH": "envoyx/envoy",
    },
    importpath = "github.com/mathetake/envoy-dynamic-modules/test/extensions/dynamic_modules/sdk/conformance-test",
    tags = ["exclusive"],
    deps = [
        "//source/extensions/dynamic_modules/sdk/go/envoy:envoy_dynamic_modules_go_sdk",
        "@com_github_testify//require",
    ],
)
