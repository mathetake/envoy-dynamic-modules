goimports := golang.org/x/tools/cmd/goimports@v0.21.0
golangci_lint := github.com/golangci/golangci-lint/cmd/golangci-lint@v1.59.0

.PHONY: build
build:
	@go build -buildmode=c-shared -o example/main.so ./example/

.PHONY: test
test:
	@CGO_ENABLED=0 go test ./...

.PHONY: conformance
conformance:
	@$$(go env GOPATH)/bin/envoy-dynamic-modules-sdk-conformance-tests --shared-library-path=./example/main.so

.PHONY: lint
lint:
	@echo "lint => ./..."
	@go run $(golangci_lint) run ./...

.PHONY: format
format:
	@echo "format => *.go"
	@find . -type f -name '*.go' | xargs gofmt -s -w
	@echo "goimports => *.go"
	@for f in `find . -name '*.go'`; do \
	    awk '/^import \($$/,/^\)$$/{if($$0=="")next}{print}' $$f > /tmp/fmt; \
	    mv /tmp/fmt $$f; \
	done
	@go run $(goimports) -w -local github.com/mathetake/envoy-dynamic-modules-go-sdk `find . -name '*.go'`

.PHONY: tidy
tidy: ## Runs go mod tidy on every module
	@find . -name "go.mod" \
	| grep go.mod \
	| xargs -I {} bash -c 'dirname {}' \
	| xargs -I {} bash -c 'echo "tidy => {}"; cd {}; go mod tidy -v; '

.PHONY: precommit
precommit: format lint tidy

.PHONY: check
check:
	@$(MAKE) precommit
	@if [ ! -z "`git status -s`" ]; then \
		echo "The following differences will fail CI until committed:"; \
		git diff --exit-code; \
	fi

all: precommit build test conformance
