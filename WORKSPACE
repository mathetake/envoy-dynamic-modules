workspace(name = "envoyx")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "envoy",
    sha256 = "39ba37aed81a9d4988a5736cf558243179f2bf1490843da25687d1aafd9d01c6",
    strip_prefix = "envoy-1.31.0",
    url = "https://github.com/envoyproxy/envoy/archive/v1.31.0.tar.gz",
)

load("@envoy//bazel:api_binding.bzl", "envoy_api_binding")

envoy_api_binding()

load("@envoy//bazel:api_repositories.bzl", "envoy_api_dependencies")

envoy_api_dependencies()

load("@envoy//bazel:repositories.bzl", "envoy_dependencies")

envoy_dependencies()

load("@envoy//bazel:repositories_extra.bzl", "envoy_dependencies_extra")

envoy_dependencies_extra()

load("@envoy//bazel:python_dependencies.bzl", "envoy_python_dependencies")

envoy_python_dependencies()

load("@envoy//bazel:dependency_imports.bzl", "envoy_dependency_imports")

envoy_dependency_imports(go_version = "1.22.4")

load("@rules_rust//crate_universe:repositories.bzl", "crate_universe_dependencies")

crate_universe_dependencies(bootstrap = True)

load("@rules_rust//crate_universe:defs.bzl", "crates_repository")

crates_repository(
    name = "rust_sdk_crate_index",
    cargo_lockfile = "//source/extensions/dynamic_modules/sdk/rust:Cargo.lock",
    generator = "@cargo_bazel_bootstrap//:cargo-bazel",
    manifests = ["//source/extensions/dynamic_modules/sdk/rust:Cargo.toml"],
)

load("@rust_sdk_crate_index//:defs.bzl", crate_repositories_crate_repositories = "crate_repositories")

crate_repositories_crate_repositories()

load("@bazel_gazelle//:deps.bzl", "go_repository")

go_repository(
    name = "com_github_testify",
    importpath = "github.com/stretchr/testify",
    sum = "h1:HtqpIVDClZ4nwg75+f6Lvsy/wHu+3BoSGCbBAcpTsTg=",
    version = "v1.9.0",
)
