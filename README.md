# Envoy + dynamic modules 

This is the repository that hosts the custom C++ code that is compiled togather with the Envoy Proxy code to allow dynamically loadable modules to be loaded as HTTP filters.

We are in the process of upstreaming the work here to the main Envoy Proxy repository. Meanwhile, this repository serves as a playground to experiment with the dynamic modules feature.

* [abi](./abi/) directory contains the ABI definitions for the dynamic modules, which is a single plain C header with zero dependency.
* [x](./x/) directory contains the extension code, that will be compiled into the final Envoy binary.
* [sdks/conformance-test](./sdks/conformance-test) directory contains the conformance tests for the SDKs.
* [sdks/go](./sdks/go/) directory contains the Go SDK for the dynamic modules.

TODO: SDKs and conformance tests will be soon merged into this repository.

## Links

- [mathetake/envoy-dynamic-modules-rust-sdk](https://github.com/mathetake/envoy-dynamic-modules-rust-sdk): The Rust SDK for the dynamic modules.
