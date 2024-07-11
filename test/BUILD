cc_test(
    name = "dynamic_module_test",
    srcs = ["dynamic_module_test.cc"],
    data = [
        "//test/test_programs:init",
        "//test/test_programs:init_fail",
        "//test/test_programs:no_init",
    ],
    linkopts = [
        "-ldl",
    ],
    deps = [
        "//x:dynamic_module_lib",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
        "@envoy//test/test_common:test_version_linkstamp",
        # ^^ required by utility_lib.
        "@envoy//test/test_common:utility_lib",
    ],
)
