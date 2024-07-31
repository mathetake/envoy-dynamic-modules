package main

import (
	"github.com/mathetake/envoy-dynamic-modules/sdks/go/envoy"
)

func main() {} // main function must be present but empty.

// Set the envoy.NewHttpFilter function to create a new http filter.
func init() { envoy.NewHttpFilter = newHttpFilter }

// newHttpFilter creates a new http filter based on the config.
//
// `config` is the configuration string that is specified in the Envoy configuration.
func newHttpFilter(config string) envoy.HttpFilter {
	switch config {
	case "helloworld":
		return newHelloWorldHttpFilter(config)
	case "delay":
		return newDelayHttpFilter(config)
	case "headers":
		return newHeadersHttpFilter(config)
	case "bodies":
		return newbodiesHttpFilter(config)
	case "bodies_replace":
		return newbodiesReplaceHttpFilter(config)
	case "send_response":
		return newSendResponseFilter(config)
	case "validate_json":
		return newValidateJsonFilter(config)
	default:
		panic("unknown filter: " + config)
	}
}
