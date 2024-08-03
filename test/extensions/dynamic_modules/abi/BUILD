sh_test(
    name = "compare_abi_headers",
    srcs = ["compare_abi_headers.sh"],
    args = [
        "$(location //source/extensions/dynamic_modules/abi:abi.h)",
        "$(location //source/extensions/dynamic_modules/sdks/go/envoy:abi.h)",
        "$(location //source/extensions/dynamic_modules/sdks/rust:abi.h)",
    ],
    data = [
        "compare_abi_headers.sh",
        "//source/extensions/dynamic_modules/abi:abi.h",
        "//source/extensions/dynamic_modules/sdks/go/envoy:abi.h",
        "//source/extensions/dynamic_modules/sdks/rust:abi.h",
    ],
)
