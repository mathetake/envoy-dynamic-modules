load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_test",
)

LINK_OPTS = [
    "-ldl",
]

COPTS = [
    "-DENVOY_DYNAMIC_MODULE=1",
]

DEPS = [
    "//test:test_util",
    "@com_google_googletest//:gtest",
    "@com_google_googletest//:gtest_main",
    "@envoy//test/test_common:test_version_linkstamp",
    # ^^ required by utility_lib.
    "@envoy//test/test_common:utility_lib",
]

cc_library(
    name = "test_util",
    hdrs = ["test_util.h"],
    copts = COPTS,
    deps = [
        "//x:dynamic_module_lib",
    ],
)

cc_test(
    name = "dynamic_module_test",
    srcs = ["dynamic_module_test.cc"],
    copts = COPTS,
    data = [
        "//test/test_programs:http_filter_init_fail",
        "//test/test_programs:init",
        "//test/test_programs:no_init",
        "//test/test_programs:program_init_fail",
        "//test/test_programs:stream_init",
    ],
    linkopts = LINK_OPTS,
    deps = [
        "//x:dynamic_module_lib",
    ] + DEPS,
)

cc_test(
    name = "filter_test",
    srcs = ["filter_test.cc"],
    copts = COPTS,
    data = [
        "//test/test_programs:init",
        "//test/test_programs:stream_init",
    ],
    linkopts = LINK_OPTS,
    deps = [
        "//x:filter_lib",
    ] + DEPS,
)

cc_test(
    name = "abi_test",
    srcs = ["abi_test.cc"],
    copts = COPTS,
    data = [
        "//test/test_programs:get_body",
        "//test/test_programs:get_headers",
        "//test/test_programs:manipulate_body",
        "//test/test_programs:set_headers",
    ],
    linkopts = LINK_OPTS,
    deps = [
        "//x:abi_lib",
    ] + DEPS,
)

envoy_cc_test(
    name = "integration_test",
    srcs = ["integration_test.cc"],
    data = [
        "//test/test_programs:integration_test_headers",
    ],
    repository = "@envoy",
    deps = [
        "//x:factory",
        "@envoy//test/integration:http_integration_lib",
    ],
)
