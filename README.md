# EnvoyX: Envoy Proxy meets Dynamically Loadable Modules (WIP)

```
# Build.
bazel build //:envoy

# Formatting.
clang-format -i x/*.cc x/*.h test/*.cc test/*.h

# Clang-Tidy
bazel build --config=clang-tidy $(bazel query '//test:*') //x/...

# Test.
bazel test //...
```
