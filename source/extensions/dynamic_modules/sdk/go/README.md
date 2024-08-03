# Envoy dynamic modules Go SDK

This is the Go SDK for the dynamic modules. The modules are shared libraries that can be loaded by the Envoy to extend HTTP filtering capabilities.

Since only one Go-based shared library can exist in a process due to [the limitation of the Go runtime](https://github.com/golang/go/issues/65050),
this SDK facilitates the creation of Go-based shared libraries that can be loaded at multiple HTTP filter chain
in Envoy configuration. See the [example](./example) for more details.

