# EnvoyX: Envoy Proxy meets Dynamically Loadable Modules

This is the repository that host the custom C++ code that is compiled togather with the Envoy Proxy code to allow
dynamically loadable modules to be loaded into the Envoy Proxy.

The ABI is hosted at [envoyproxyx/abi](https://github.com/envoyproxyx/abi).

## Development

This repository comes with the devcontainer configuration that allows you to develop in a containerized environment.

```bash
# Build Envoy Proxy b
bazel build //:envoy

# Formatting.
clang-format -i x/*.cc x/*.h test/*.cc test/*.h

# clang-tidy.
bazel build --config=clang-tidy $(bazel query '//test:*') //x/...

# Test.
bazel test //...
```
