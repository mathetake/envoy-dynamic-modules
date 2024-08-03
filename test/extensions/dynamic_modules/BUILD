LINK_OPTS = [
    "-ldl",
]

COPTS = [
    "-DENVOY_DYNAMIC_MODULE=1",
]

DEPS = [
    "@com_google_googletest//:gtest",
    "@com_google_googletest//:gtest_main",
    "@envoy//test/test_common:test_version_linkstamp",
    # ^^ required by utility_lib.
    "@envoy//test/test_common:utility_lib",
]

cc_test(
    name = "dynamic_modules_test",
    srcs = ["dynamic_modules_test.cc"],
    copts = COPTS,
    data = [
        "//test/extensions/dynamic_modules/test_programs:no_op",
    ],
    linkopts = LINK_OPTS,
    deps = [
        "//source/extensions/dynamic_modules:dynamic_modules_lib",
    ] + DEPS,
)
