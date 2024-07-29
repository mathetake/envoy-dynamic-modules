# Envoy + dynamic modules 

This is the repository that hosts the custom C++ code that is compiled togather with the Envoy Proxy code to allow dynamically loadable modules to be loaded as HTTP filters.

* `abi` directory contains the ABI definitions for the dynamic modules, which is a single plane C header with zero dependency.
* `x` directory contains the extension code.

TODO: SDKs and conformance tests will be soon merged into this repository.

## Links

- [mathetake/envoy-dynamic-modules-go-sdk](https://github.com/mathetake/envoy-dynamic-modules-go-sdk): The Go SDK for the dynamic modules.
- [mathetake/envoy-dynamic-modules-rust-sdk](https://github.com/mathetake/envoy-dynamic-modules-rust-sdk): The Rust SDK for the dynamic modules.
- [mathetake/envoy-dynamic-modules-sdk-conformance-tests](https://github.com/mathetake/envoy-dynamic-modules-sdk-conformance-tests): The SDK conformance tests for the Envoy dynamic modules.
