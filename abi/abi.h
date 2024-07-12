namespace Envoy {
namespace Http {
namespace DynamicModule {
namespace ABI {

// EnvoyFilterPtr is a pointer to the DynamicModule::HttpFilter instance. It is always passed to the
// module's event hooks. Modules are not supposed to manipulate this pointer.
using EnvoyFilterPtr = void*;

// ModuleContextPtr is a pointer to in-module context corresponding to a single
// DynamicModule::HttpFilter instance. It is always passed to the module's event hooks.
using ModuleContextPtr = void*;

// RequestHeadersPtr is a pointer to the RequestHeaderMap instance. This is passed to the
// __envoy_module_http_on_request_headers event hook.
// Modules are not supposed to manipulate this pointer.
using RequestHeadersPtr = void*;

// OnRequestHeadersStatus is the return value of the __envoy_module_http_on_request_headers event.
// It should be one of the values defined in the FilterHeadersStatus enum.
using OnRequestHeadersStatus = int;

// ResponseHeaderMapPtr is a pointer to the ResponseHeaderMap instance. This is passed to the
// __envoy_module_http_on_response_headers event hook.
// Modules are not supposed to manipulate this pointer.
using ResponseHeaderMapPtr = void*;

// OnResponseHeadersStatus is the return value of the __envoy_module_http_on_response_headers event.
// It should be one of the values defined in the FilterHeadersStatus enum.
using OnResponseHeadersStatus = int;

// RequestBufferPtr is a pointer to the Buffer::Instance instance. This is passed to the
// __envoy_module_http_on_request_body event hook.
// Modules are not supposed to manipulate this pointer.
using RequestBufferPtr = void*;

// OnRequestBodyStatus is the return value of the __envoy_module_http_on_request_body event.
// It should be one of the values defined in the FilterDataStatus enum.
using OnRequestBodyStatus = int;

// ResponseBufferPtr is a pointer to the Buffer::Instance instance. This is passed to the
// __envoy_module_http_on_response_body event hook.
// Modules are not supposed to manipulate this pointer.
using ResponseBufferPtr = void*;

// OnResponseBodyStatus is the return value of the __envoy_module_http_on_response_body event.
// It should be one of the values defined in the FilterDataStatus enum.
using OnResponseBodyStatus = int;

// __envoy_module_init is called by the main thread when the module is loaded exactly once per
// module. The function should return 0 on success and non-zero on failure.
constexpr char __envoy_module_init[] = "__envoy_module_init";
using EnvoyModuleInitSig = int (*)(const char*);

// __envoy_module_http_context_init is called by any worker thread when a new stream is
// created. That means that the function should be thread-safe.
//
// The function should return a pointer to a new instance of the context or nullptr on failure.
// The lifetime of the returned pointer should be managed by the dynamic module.
constexpr char __envoy_module_http_context_init[] = "__envoy_module_http_context_init";
using EnvoyModuleHttpContextInitSig = ModuleContextPtr (*)();

// __envoy_module_http_on_request_headers is called when request headers are received.
constexpr char __envoy_module_http_on_request_headers[] = "__envoy_module_http_on_request_headers";
using EnvoyModuleHttpOnRequestHeadersSig = OnRequestHeadersStatus (*)(EnvoyFilterPtr,
                                                                      ModuleContextPtr,
                                                                      RequestHeadersPtr,
                                                                      bool /* End of stream */);

// __envoy_module_http_on_request_body is called when request body data is received.
constexpr char __envoy_module_http_on_request_body[] = "__envoy_module_http_on_request_body";
using EnvoyModuleHttpOnRequestBodySig = OnRequestBodyStatus (*)(EnvoyFilterPtr, ModuleContextPtr,
                                                                RequestBufferPtr,
                                                                bool /* End of stream */);

// __envoy_module_http_on_response_headers is called when response headers are received.
constexpr char __envoy_module_http_on_response_headers[] =
    "__envoy_module_http_on_response_headers";
using EnvoyModuleHttpOnResponseHeadersSig = OnResponseHeadersStatus (*)(EnvoyFilterPtr,
                                                                        ModuleContextPtr,
                                                                        ResponseHeaderMapPtr,
                                                                        bool /* End of stream */);

// __envoy_module_http_on_response_body is called when response body data is received.
constexpr char __envoy_module_http_on_response_body[] = "__envoy_module_http_on_response_body";
using EnvoyModuleHttpOnResponseBodySig = OnResponseBodyStatus (*)(EnvoyFilterPtr, ModuleContextPtr,
                                                                  ResponseBufferPtr,
                                                                  bool /* End of stream */);

// __envoy_module_http_on_destroy is called when the stream is destroyed.
constexpr char __envoy_module_http_on_destroy[] = "__envoy_module_http_on_destroy";
using EnvoyModuleHttpOnDestroySig = void (*)(EnvoyFilterPtr, ModuleContextPtr);

} // namespace ABI
} // namespace DynamicModule
} // namespace Http
} // namespace Envoy
