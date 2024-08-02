load("@rules_rust//cargo:defs.bzl", "cargo_build_script")
load("@rules_rust//rust:defs.bzl", "rust_library", "rust_shared_library", "rust_test")
load("@rust_sdk_crate_index//:defs.bzl", "all_crate_deps")

exports_files(["abi.h"])  # Exported for tests.

cargo_build_script(
    name = "build_script",
    srcs = ["build.rs"],
    data = [
        "abi.h",
    ],
    edition = "2021",
    deps = all_crate_deps(
        build = True,
        normal = True,
    ),
)

rust_library(
    name = "envoy_dynamic_modules_rust_sdk",
    srcs = ["src/lib.rs"],
    edition = "2021",
    deps = all_crate_deps(
        normal = True,
    ) + [":build_script"],
)

rust_shared_library(
    name = "example",
    srcs = ["example/example.rs"],
    edition = "2021",
    visibility = ["//visibility:public"],
    deps = all_crate_deps(
        normal = True,
    ) + [
        ":build_script",
        ":envoy_dynamic_modules_rust_sdk",
    ],
)

rust_test(
    name = "envoy_dynamic_modules_rust_sdk_test",
    crate = ":envoy_dynamic_modules_rust_sdk",
)

rust_test(
    name = "example_test",
    crate = ":example",
)
