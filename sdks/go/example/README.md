# Envoy Dynamic Module Go SDK example

This example demonstrates how to use the Go SDK to create HTTP filters for Envoy. This example is supposed to be compiled as a
single shared library but to server multiple HTTP filters. See [envoy.yaml](envoy.yaml) for the configuration.

In main.go, this multiplexes the different HTTP filters based on the `filter_config` parameter given in the Envoy configuration.
Each file named `filter_<name>.go` is a separate HTTP filter implementation which is run on the separater HTTP filter chain.

Note that this example is written in a way that it passes the [sdk-conformance-tests](https://github.com/envoyproxyx/sdk-conformance-tests) and can be used as a reference for using Go SDK APIs.

## Build and Run

To build and run the example, run the following, assuming you have `envoy` installed as exaplined in [README.md](../README.md):

```
CGO_ENABLED=1 GOOS=linux GOARCH=amd64 go build -buildmode=c-shared -o main.so .
envoy -c envoy.yaml
```
