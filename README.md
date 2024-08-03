# Envoy + dynamic modules 

This is the repository that hosts the custom C++ code that is compiled togather with the Envoy Proxy code to allow dynamically loadable modules to be loaded as HTTP filters.

We are already in the process of upstreaming the work here to the main Envoy Proxy repository. Meanwhile, this repository serves as a playground to experiment with the dynamic modules feature.

To align the development with the upstream, all source code is put under [source/extensions/dynamic_modules](source/extensions/dynamic_modules) directory.

For SDKs, follow the links below (will change soon though):

* [sdks/conformance-test](./sdks/conformance-test) directory contains the conformance tests for the SDKs.
* [sdks/go](./sdks/go/) directory contains the Go SDK for the dynamic modules.
* [sdks/rust](./sdks/rust/) directory contains the Rust SDK for the dynamic modules.
