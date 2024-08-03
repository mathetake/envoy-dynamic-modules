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
    "//test/extensions/dynamic_modules/http:test_util",
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
        "//source/extensions/dynamic_modules/http:http_dynamic_module_lib",
    ],
)

cc_test(
    name = "http_dynamic_module_test",
    srcs = ["http_dynamic_module_test.cc"],
    copts = COPTS,
    data = [
        "//test/extensions/dynamic_modules/http/test_programs:do_not_close",
        "//test/extensions/dynamic_modules/http/test_programs:http_filter_init_fail",
        "//test/extensions/dynamic_modules/http/test_programs:init",
        "//test/extensions/dynamic_modules/http/test_programs:no_init",
        "//test/extensions/dynamic_modules/http/test_programs:program_init_fail",
        "//test/extensions/dynamic_modules/http/test_programs:stream_init",
    ],
    linkopts = LINK_OPTS,
    deps = [
        "//source/extensions/dynamic_modules/http:http_dynamic_module_lib",
    ] + DEPS,
)

cc_test(
    name = "filter_test",
    srcs = ["filter_test.cc"],
    copts = COPTS,
    data = [
        "//test/extensions/dynamic_modules/http/test_programs:init",
        "//test/extensions/dynamic_modules/http/test_programs:stream_init",
    ],
    linkopts = LINK_OPTS,
    deps = [
        "//source/extensions/dynamic_modules/http:filter_lib",
    ] + DEPS,
)

cc_test(
    name = "abi_test",
    srcs = ["abi_test.cc"],
    copts = COPTS,
    data = [
        "//test/extensions/dynamic_modules/http/test_programs:get_body",
        "//test/extensions/dynamic_modules/http/test_programs:get_headers",
        "//test/extensions/dynamic_modules/http/test_programs:manipulate_body",
        "//test/extensions/dynamic_modules/http/test_programs:set_headers",
    ],
    linkopts = LINK_OPTS,
    deps = [
        "//source/extensions/dynamic_modules/http:abi_lib",
    ] + DEPS,
)

envoy_cc_test(
    name = "integration_test",
    srcs = ["integration_test.cc"],
    data = [
        "//test/extensions/dynamic_modules/http/test_programs:integration_test_bodies",
        "//test/extensions/dynamic_modules/http/test_programs:integration_test_headers",
        "//test/extensions/dynamic_modules/http/test_programs:integration_test_local_response",
    ],
    repository = "@envoy",
    deps = [
        "//source/extensions/dynamic_modules/http:factory",
        "@envoy//test/integration:http_integration_lib",
    ],
)
