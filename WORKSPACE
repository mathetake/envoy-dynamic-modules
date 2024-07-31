workspace(name = "envoyx")

load("@envoyx//bazel:repositories.bzl", "repositories")

repositories()

# Envoy deps

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
    cargo_lockfile = "//sdks/rust:Cargo.lock",
    generator = "@cargo_bazel_bootstrap//:cargo-bazel",
    manifests = ["//sdks/rust:Cargo.toml"],
)

load("@rust_sdk_crate_index//:defs.bzl", crate_repositories_crate_repositories = "crate_repositories")

crate_repositories_crate_repositories()

load("@bazel_gazelle//:deps.bzl", "go_repository")

# Download using "go mod download"
go_repository(
    name = "com_github_testify",
    importpath = "github.com/stretchr/testify",
    sum = "h1:HtqpIVDClZ4nwg75+f6Lvsy/wHu+3BoSGCbBAcpTsTg=",
    version = "v1.9.0",
)
