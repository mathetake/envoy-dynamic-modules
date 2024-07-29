goimports := golang.org/x/tools/cmd/goimports@v0.21.0
golangci_lint := github.com/golangci/golangci-lint/cmd/golangci-lint@v1.59.0

.PHONY: clippy
clippy:
	@cargo clippy --all --examples  -- -D warnings

.PHONY: fmt
fmt:
	@cargo fmt --all -- --check

.PHONY: build
build:
	@cargo build
	@cargo build --examples

.PHONY: test
test:
	@cargo test
	@cargo test --examples

.PHONY: conformance
conformance:
	@$$(go env GOPATH)/bin/envoy-dynamic-modules-sdk-conformance-tests --shared-library-path=./target/debug/examples/libexample.so

all: clippy fmt build test conformance
