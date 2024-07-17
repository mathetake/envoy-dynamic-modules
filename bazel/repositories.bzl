load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

ENVOY_VERSION = "1.30.4-light"  # Change this to the desired Envoy version. -light is optional.

ENVOY_VERSIONS = {
    "1.30.4": {
        "sha256": "f334be38316bc38d93459dd3f2d1a356c5094d975438cdc0d92b7f36175bdfa9",
        "url": "https://github.com/envoyproxy/envoy/archive/v1.30.4.tar.gz",
        "strip_prefix": "envoy-1.30.4",
        "extensions_build_config_dir": "bazel/light_envoy_extensions_build_configs/1.30",
    },
    "1.29.7": {
        "sha256": "640f97362ccfe07eaf0ac83c0050d5348f49d10d05e22072091aef2af602e9ec",
        "url": "https://github.com/envoyproxy/envoy/archive/v1.29.7.tar.gz",
        "strip_prefix": "envoy-1.29.7",
        "extensions_build_config_dir": "bazel/light_envoy_extensions_build_configs/1.29",
    },
}

def repositories():
    splits = ENVOY_VERSION.split("-")
    version, is_light = "", False
    if len(splits) == 1:
        version = splits[0]
    elif len(splits) == 2:
        version = splits[0]
        is_light = splits[1] == "light"
    else:
        fail("Invalid Envoy version: %s" % ENVOY_VERSION)

    # check if the version is in the ENVOY_VERSIONS dict.
    if version not in ENVOY_VERSIONS:
        fail("Unsupported Envoy version: %s" % version)

    values = ENVOY_VERSIONS[version]

    if is_light:
        # Replace the default config by the light extensions build config repository for the given version.
        native.new_local_repository(
            name = "envoy_build_config",
            path = values["extensions_build_config_dir"],
            build_file_content = "",
        )

    http_archive(
        name = "envoy",
        sha256 = values["sha256"],
        strip_prefix = values["strip_prefix"],
        urls = [values["url"]],
    )
