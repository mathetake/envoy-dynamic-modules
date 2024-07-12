namespace Envoy {
namespace Http {
namespace DynamicModule {
namespace ABI {

// EnvoyFilterPtr is a pointer to the DynamicModule::HttpFilter instance. It is always passed to the
// module's event hooks.
using EnvoyFilterPtr = void*;
// ModuleContextPtr is a pointer to in-module context corresponding to a single
// DynamicModule::HttpFilter instance. It is always passed to the module's event hooks.
using ModuleContextPtr = void*;

// __envoy_module_init is called by the main thread when the module is loaded exactly once per
// module. The function should return 0 on success and non-zero on failure.
constexpr char __envoy_module_init[] = "__envoy_module_init";
using EnvoyModuleInit = int (*)(const char*);

// __envoy_module_http_context_init is called by any worker thread when a new stream is
// created. That means that the function should be thread-safe.
//
// The function should return a pointer to a new instance of the context or nullptr on failure.
// The lifetime of the returned pointer should be managed by the dynamic module.
constexpr char __envoy_module_http_context_init[] = "__envoy_module_http_context_init";
using EnvoyModuleHttpContextInit = ModuleContextPtr (*)();

// __envoy_module_http_on_request_headers is called when request headers are received.
constexpr char __envoy_module_http_on_request_headers[] = "__envoy_module_http_on_request_headers";
using EnvoyModuleHttpOnRequestHeaders = int (*)(EnvoyFilterPtr, ModuleContextPtr);

// __envoy_module_http_on_request_body is called when request body data is received.
constexpr char __envoy_module_http_on_request_body[] = "__envoy_module_http_on_request_body";
using EnvoyModuleHttpOnRequestBody = int (*)(EnvoyFilterPtr, ModuleContextPtr);

// __envoy_module_http_on_response_headers is called when response headers are received.
constexpr char __envoy_module_http_on_response_headers[] =
    "__envoy_module_http_on_response_headers";
using EnvoyModuleHttpOnResponseHeaders = int (*)(EnvoyFilterPtr, ModuleContextPtr);

// __envoy_module_http_on_response_body is called when response body data is received.
constexpr char __envoy_module_http_on_response_body[] = "__envoy_module_http_on_response_body";
using EnvoyModuleHttpOnResponseBody = int (*)(EnvoyFilterPtr, ModuleContextPtr);

// __envoy_module_http_on_destroy is called when the stream is destroyed.
constexpr char __envoy_module_http_on_destroy[] = "__envoy_module_http_on_destroy";
using EnvoyModuleHttpOnDestroy = void (*)(EnvoyFilterPtr, ModuleContextPtr);

} // namespace ABI
} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
