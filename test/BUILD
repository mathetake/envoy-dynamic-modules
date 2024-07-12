LINK_OPTS = [
    "-ldl",
]

DEPS = [
    "@com_google_googletest//:gtest",
    "@com_google_googletest//:gtest_main",
    "@envoy//test/test_common:test_version_linkstamp",
    # ^^ required by utility_lib.
    "@envoy//test/test_common:utility_lib",
]

cc_test(
    name = "dynamic_module_test",
    srcs = ["dynamic_module_test.cc"],
    data = [
        "//test/test_programs:init",
        "//test/test_programs:init_fail",
        "//test/test_programs:no_init",
    ],
    linkopts = LINK_OPTS,
    deps = [
        "//x:dynamic_module_lib",
    ] + DEPS,
)

cc_test(
    name = "filter_test",
    srcs = ["filter_test.cc"],
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
    name = "abi_impl_test",
    srcs = ["abi_impl_test.cc"],
    linkopts = LINK_OPTS,
    deps = [
        "//abi:abi_lib",
        "//x:abi_impl_lib",
    ] + DEPS,
)
