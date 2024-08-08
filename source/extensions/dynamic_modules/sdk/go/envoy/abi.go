//go:build cgo

package envoy

/*
#include "abi.h"
*/
import "C"
import (
	"bytes"
	"io"
	"runtime"
	"unicode/utf8"
	"unsafe"
)

//export envoy_dynamic_module_on_program_init
func envoy_dynamic_module_on_program_init() C.size_t {
	return 0
}

//export envoy_dynamic_module_on_http_filter_init
func envoy_dynamic_module_on_http_filter_init(
	configPtr C.envoy_dynamic_module_type_HttpFilterConfigPtr,
	configSize C.envoy_dynamic_module_type_HttpFilterConfigSize) C.envoy_dynamic_module_type_HttpFilterPtr {
	rawStr := unsafe.String((*byte)(unsafe.Pointer(uintptr(configPtr))), configSize)
	// Copy the config string to Go memory so that the caller can take ownership of the memory.
	var configStrCopy = make([]byte, len(rawStr))
	copy(configStrCopy, rawStr)
	// Call the exported function from the Go module.
	httpFilter := NewHttpFilter(rawStr)
	pined := memManager.pinHttpFilter(httpFilter)
	return C.envoy_dynamic_module_type_HttpFilterPtr((uintptr)(unsafe.Pointer(pined)))
}

//export envoy_dynamic_module_on_http_filter_destroy
func envoy_dynamic_module_on_http_filter_destroy(
	httpFilterPtr C.envoy_dynamic_module_type_HttpFilterPtr) {
	httpFilter := memManager.unwrapPinnedHttpFilter(uintptr(httpFilterPtr))
	httpFilter.obj.Destroy()
	memManager.unpinHttpFilter(httpFilter)
}

//export envoy_dynamic_module_on_http_filter_instance_init
func envoy_dynamic_module_on_http_filter_instance_init(
	envoyFilterPtr C.envoy_dynamic_module_type_EnvoyFilterInstancePtr,
	httpFilterPtr C.envoy_dynamic_module_type_HttpFilterPtr,
) C.envoy_dynamic_module_type_HttpFilterInstancePtr {
	envoyPtr := EnvoyFilterInstance{raw: envoyFilterPtr}
	httpFilter := memManager.unwrapPinnedHttpFilter(uintptr(httpFilterPtr))
	httpInstance := httpFilter.obj.NewInstance(envoyPtr)
	pined := memManager.pinHttpFilterInstance(httpInstance)
	return C.envoy_dynamic_module_type_HttpFilterInstancePtr(uintptr((unsafe.Pointer(pined))))
}

//export envoy_dynamic_module_on_http_filter_instance_request_headers
func envoy_dynamic_module_on_http_filter_instance_request_headers(
	httpFilterInstancePtr C.envoy_dynamic_module_type_HttpFilterInstancePtr,
	requestHeadersPtr C.envoy_dynamic_module_type_HttpRequestHeadersMapPtr,
	endOfStream C.envoy_dynamic_module_type_EndOfStream,
) C.envoy_dynamic_module_type_EventHttpRequestHeadersStatus {
	httpInstance := unwrapRawPinHttpFilterInstance(uintptr(httpFilterInstancePtr))
	mapPtr := RequestHeaders{raw: requestHeadersPtr}
	end := endOfStream != 0
	result := httpInstance.obj.RequestHeaders(mapPtr, end)
	return C.envoy_dynamic_module_type_EventHttpRequestHeadersStatus(result)
}

//export envoy_dynamic_module_on_http_filter_instance_request_body
func envoy_dynamic_module_on_http_filter_instance_request_body(
	httpFilterInstancePtr C.envoy_dynamic_module_type_HttpFilterInstancePtr,
	buffer C.envoy_dynamic_module_type_HttpRequestBodyBufferPtr,
	endOfStream C.envoy_dynamic_module_type_EndOfStream) C.envoy_dynamic_module_type_EventHttpRequestBodyStatus {
	httpInstance := unwrapRawPinHttpFilterInstance(uintptr(httpFilterInstancePtr))
	buf := RequestBodyBuffer{raw: buffer}
	end := endOfStream != 0
	result := httpInstance.obj.RequestBody(buf, end)
	return C.envoy_dynamic_module_type_EventHttpRequestBodyStatus(result)
}

//export envoy_dynamic_module_on_http_filter_instance_response_headers
func envoy_dynamic_module_on_http_filter_instance_response_headers(
	httpFilterInstancePtr C.envoy_dynamic_module_type_HttpFilterInstancePtr,
	responseHeadersMapPtr C.envoy_dynamic_module_type_HttpResponseHeaderMapPtr,
	endOfStream C.envoy_dynamic_module_type_EndOfStream) C.envoy_dynamic_module_type_EventHttpResponseHeadersStatus {
	httpInstance := unwrapRawPinHttpFilterInstance(uintptr(httpFilterInstancePtr))
	mapPtr := ResponseHeaders{raw: responseHeadersMapPtr}
	end := endOfStream != 0
	result := httpInstance.obj.ResponseHeaders(mapPtr, end)
	return C.envoy_dynamic_module_type_EventHttpResponseHeadersStatus(result)
}

//export envoy_dynamic_module_on_http_filter_instance_response_body
func envoy_dynamic_module_on_http_filter_instance_response_body(
	httpFilterInstancePtr C.envoy_dynamic_module_type_HttpFilterInstancePtr,
	buffer C.envoy_dynamic_module_type_HttpResponseBodyBufferPtr,
	endOfStream C.envoy_dynamic_module_type_EndOfStream) C.envoy_dynamic_module_type_EventHttpResponseBodyStatus {
	httpInstance := unwrapRawPinHttpFilterInstance(uintptr(httpFilterInstancePtr))
	buf := ResponseBodyBuffer{raw: buffer}
	end := endOfStream != 0
	result := httpInstance.obj.ResponseBody(buf, end)
	return C.envoy_dynamic_module_type_EventHttpResponseBodyStatus(result)
}

//export envoy_dynamic_module_on_http_filter_instance_destroy
func envoy_dynamic_module_on_http_filter_instance_destroy(
	httpFilterInstancePtr C.envoy_dynamic_module_type_HttpFilterInstancePtr) {
	httpInstance := unwrapRawPinHttpFilterInstance(uintptr(httpFilterInstancePtr))
	httpInstance.obj.Destroy()
	memManager.unpinHttpFilterInstance((*pinedHttpFilterInstance)(unsafe.Pointer(uintptr(httpFilterInstancePtr))))
}

// envoyFilterInstance implements the EnvoyFilterInstance interface in abi_nocgo.go which is not included in the shared library.
type EnvoyFilterInstance struct {
	raw C.envoy_dynamic_module_type_EnvoyFilterInstancePtr
}

// ContinueRequest implements EnvoyFilterInstance interface in abi_nocgo.go which is not included in the shared library.
func (c EnvoyFilterInstance) ContinueRequest() {
	C.envoy_dynamic_module_http_continue_request(c.raw)
}

// ContinueResponse implements EnvoyFilterInstance interface in abi_nocgo.go which is not included in the shared library.
func (c EnvoyFilterInstance) ContinueResponse() {
	C.envoy_dynamic_module_http_continue_response(c.raw)
}

// GetRequestBodyBuffer implements EnvoyFilterInstance interface in abi_nocgo.go which is not included in the shared library.
func (c EnvoyFilterInstance) GetRequestBodyBuffer() RequestBodyBuffer {
	return RequestBodyBuffer{raw: C.envoy_dynamic_module_http_get_request_body_buffer(c.raw)}
}

// GetResponseBodyBuffer implements EnvoyFilterInstance interface in abi_nocgo.go which is not included in the shared library.
func (c EnvoyFilterInstance) GetResponseBodyBuffer() ResponseBodyBuffer {
	return ResponseBodyBuffer{raw: C.envoy_dynamic_module_http_get_response_body_buffer(c.raw)}
}

// SendResponse implements EnvoyFilterInstance interface in abi_nocgo.go which is not included in the shared library.
func (c EnvoyFilterInstance) SendResponse(statusCode int, headers [][2]string, body []byte) {
	headersLen := len(headers)
	var headersPtr uintptr
	if headersLen > 0 {
		headersPtr = uintptr(unsafe.Pointer(&headers[0]))
	}
	bodyLen := len(body)
	var bodyPtr uintptr
	if bodyLen > 0 {
		bodyPtr = uintptr(unsafe.Pointer(&body[0]))
	}
	C.envoy_dynamic_module_http_send_response(c.raw,
		C.uint32_t(statusCode),
		C.envoy_dynamic_module_type_InModuleBufferPtr(headersPtr),
		C.envoy_dynamic_module_type_InModuleBufferLength(headersLen),
		C.envoy_dynamic_module_type_InModuleBufferPtr(bodyPtr),
		C.envoy_dynamic_module_type_InModuleBufferLength(bodyLen),
	)
}

// RequestHeaders implements RequestHeaders interface in abi_nocgo.go which is not included in the shared library.
type RequestHeaders struct {
	raw C.envoy_dynamic_module_type_HttpRequestHeadersMapPtr
}

// ResponseHeaders implements ResponseHeaders interface in abi_nocgo.go which is not included in the shared library.
type ResponseHeaders struct {
	raw C.envoy_dynamic_module_type_HttpResponseHeaderMapPtr
}

// RequestBodyBuffer implements RequestBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
type RequestBodyBuffer struct {
	raw C.envoy_dynamic_module_type_HttpRequestBodyBufferPtr
}

// ResponseBodyBuffer implements ResponseBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
type ResponseBodyBuffer struct {
	raw C.envoy_dynamic_module_type_HttpResponseBodyBufferPtr
}

// Get implements RequestHeaders interface in abi_nocgo.go which is not included in the shared library.
func (r RequestHeaders) Get(key string) (HeaderValue, bool) {
	// Take the raw pointer to the key by using unsafe.
	keyPtr := uintptr(unsafe.Pointer(unsafe.StringData(key)))
	keySize := len(key)

	var resultPtr *byte
	var resultSize int
	total := C.envoy_dynamic_module_http_get_request_header_value(r.raw,
		C.envoy_dynamic_module_type_InModuleBufferPtr(keyPtr),
		C.envoy_dynamic_module_type_InModuleBufferLength(keySize),
		C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&resultPtr))),
		C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&resultSize))),
	)
	if total == 0 {
		return HeaderValue{}, false
	}
	runtime.KeepAlive(key)
	return HeaderValue{data: resultPtr, size: int(resultSize)}, true
}

// Values implements RequestHeaders interface in abi_nocgo.go which is not included in the shared library.
func (r RequestHeaders) Values(key string, iter func(value HeaderValue)) {
	// Take the raw pointer to the key by using unsafe.
	keyPtr := uintptr(unsafe.Pointer(unsafe.StringData(key)))
	keySize := len(key)

	var resultPtr *byte
	var resultSize int
	total := C.envoy_dynamic_module_http_get_request_header_value(r.raw,
		C.envoy_dynamic_module_type_InModuleBufferPtr(keyPtr),
		C.envoy_dynamic_module_type_InModuleBufferLength(keySize),
		C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&resultPtr))),
		C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&resultSize))),
	)
	if total == 0 {
		return
	}

	iter(HeaderValue{data: resultPtr, size: int(resultSize)})

	for i := 1; i < int(total); i++ {
		C.envoy_dynamic_module_http_get_request_header_value_nth(r.raw,
			C.envoy_dynamic_module_type_InModuleBufferPtr(keyPtr),
			C.envoy_dynamic_module_type_InModuleBufferLength(keySize),
			C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&resultPtr))),
			C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&resultSize))),
			C.size_t(i),
		)
		iter(HeaderValue{data: resultPtr, size: int(resultSize)})
	}
	runtime.KeepAlive(key)
}

// Set implements RequestHeaders interface in abi_nocgo.go which is not included in the shared library.
func (r RequestHeaders) Set(key, value string) {
	r.set(
		uintptr(unsafe.Pointer(unsafe.StringData(key))), len(key),
		uintptr(unsafe.Pointer(unsafe.StringData(value))), len(value),
	)
	runtime.KeepAlive(key)
	runtime.KeepAlive(value)
}

// Remove implements RequestHeaders interface in abi_nocgo.go which is not included in the shared library.
func (r RequestHeaders) Remove(key string) {
	r.set(uintptr(unsafe.Pointer(unsafe.StringData(key))), len(key), 0, 0)
	runtime.KeepAlive(key)
}

func (r RequestHeaders) set(keyPtr uintptr, keySize int, valuePtr uintptr, valueSize int) {
	C.envoy_dynamic_module_http_set_request_header(r.raw,
		C.envoy_dynamic_module_type_InModuleBufferPtr(keyPtr),
		C.envoy_dynamic_module_type_InModuleBufferLength(keySize),
		C.envoy_dynamic_module_type_InModuleBufferPtr(valuePtr),
		C.envoy_dynamic_module_type_InModuleBufferLength(valueSize),
	)
}

// Values implements ResponseHeaders interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseHeaders) Get(key string) (HeaderValue, bool) {
	// Take the raw pointer to the key by using unsafe.
	keyPtr := uintptr(unsafe.Pointer(unsafe.StringData(key)))
	keySize := len(key)

	var resultPtr *byte
	var resultSize int
	total := C.envoy_dynamic_module_http_get_response_header_value(r.raw,
		C.envoy_dynamic_module_type_InModuleBufferPtr(keyPtr),
		C.envoy_dynamic_module_type_InModuleBufferLength(keySize),
		C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&resultPtr))),
		C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&resultSize))),
	)
	if total == 0 {
		return HeaderValue{}, false
	}
	runtime.KeepAlive(key)
	return HeaderValue{data: resultPtr, size: resultSize}, true
}

// Values implements ResponseHeaders interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseHeaders) Values(key string, iter func(value HeaderValue)) {
	// Take the raw pointer to the key by using unsafe.
	keyPtr := uintptr(unsafe.Pointer(unsafe.StringData(key)))
	keySize := len(key)

	var resultPtr *byte
	var resultSize int
	total := C.envoy_dynamic_module_http_get_response_header_value(r.raw,
		C.envoy_dynamic_module_type_InModuleBufferPtr(keyPtr),
		C.envoy_dynamic_module_type_InModuleBufferLength(keySize),
		C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&resultPtr))),
		C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&resultSize))),
	)
	if total == 0 {
		return
	}

	iter(HeaderValue{data: resultPtr, size: resultSize})

	for i := 1; i < int(total); i++ {
		C.envoy_dynamic_module_http_get_response_header_value_nth(r.raw,
			C.envoy_dynamic_module_type_InModuleBufferPtr(keyPtr),
			C.envoy_dynamic_module_type_InModuleBufferLength(keySize),
			C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&resultPtr))),
			C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&resultSize))),
			C.size_t(i),
		)
		iter(HeaderValue{data: resultPtr, size: resultSize})
	}

	runtime.KeepAlive(key)
}

// Set implements ResponseHeaders interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseHeaders) Set(key, value string) {
	r.set(
		uintptr(unsafe.Pointer(unsafe.StringData(key))), len(key),
		uintptr(unsafe.Pointer(unsafe.StringData(value))), len(value),
	)
	runtime.KeepAlive(key)
	runtime.KeepAlive(value)
}

// Remove implements ResponseHeaders interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseHeaders) Remove(key string) {
	r.set(uintptr(unsafe.Pointer(unsafe.StringData(key))), len(key), 0, 0)
	runtime.KeepAlive(key)
}

func (r ResponseHeaders) set(keyPtr uintptr, keySize int, valuePtr uintptr, valueSize int) {
	C.envoy_dynamic_module_http_set_response_header(r.raw,
		C.envoy_dynamic_module_type_InModuleBufferPtr(keyPtr),
		C.envoy_dynamic_module_type_InModuleBufferLength(keySize),
		C.envoy_dynamic_module_type_InModuleBufferPtr(valuePtr),
		C.envoy_dynamic_module_type_InModuleBufferLength(valueSize),
	)
}

// Length implements RequestBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r RequestBodyBuffer) Length() int {
	return int(C.envoy_dynamic_module_http_get_request_body_buffer_length(r.raw))
}

// Slice implements RequestBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r RequestBodyBuffer) Slices(iter func(view []byte)) {
	sliceCount := r.slicesCount()
	for i := 0; i < sliceCount; i++ {
		iter(r.sliceAt(i))
	}
}

func (r RequestBodyBuffer) slicesCount() int {
	return int(C.envoy_dynamic_module_http_get_request_body_buffer_slices_count(r.raw))
}

func (r RequestBodyBuffer) sliceAt(index int) []byte {
	var ptr *byte
	var size int
	C.envoy_dynamic_module_http_get_request_body_buffer_slice(r.raw,
		C.size_t(index),
		C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&ptr))),
		C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&size))),
	)
	return unsafe.Slice(ptr, size)
}

// Copy implements RequestBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r RequestBodyBuffer) Copy() []byte {
	bytes := make([]byte, r.Length())
	offset := 0
	r.Slices(func(view []byte) {
		copy(bytes[offset:], view)
		offset += len(view)
	})
	return bytes
}

// ReadAt implements io.ReaderAt, and RequestBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r RequestBodyBuffer) ReadAt(p []byte, off int64) (n int, err error) {
	length := r.Length()
	if off >= int64(length) {
		return 0, io.EOF
	}
	diff := int64(length) - off
	if int64(len(p)) > diff {
		p = p[:diff]
		err = io.EOF
	}
	C.envoy_dynamic_module_http_copy_out_response_body_buffer(
		r.raw, C.size_t(off), C.size_t(len(p)),
		C.envoy_dynamic_module_type_InModuleBufferPtr(uintptr(unsafe.Pointer(&p[0]))),
	)
	return len(p), err
}

// Reader implements RequestBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r RequestBodyBuffer) NewReader() io.Reader {
	return &requestBufferReader{buffer: r}
}

// requestBufferReader implements io.Reader for the RequestBodyBuffer.
type requestBufferReader struct {
	buffer             RequestBodyBuffer
	currentSliceIndex  int
	currentSliceOffset int
}

// Read implements io.Reader for the RequestBodyBuffer.
func (r *requestBufferReader) Read(buf []byte) (int, error) {
	totalRead := 0
	slicesCount := r.buffer.slicesCount()
	// TODO: below obviously can be optimized. But for now, this is fine for the PoC.
	for totalRead < len(buf) {
		if r.currentSliceIndex >= slicesCount {
			return totalRead, io.EOF
		}
		currentSlice := r.buffer.sliceAt(r.currentSliceIndex)
		currentSliceLen := len(currentSlice)
		if r.currentSliceOffset >= currentSliceLen {
			r.currentSliceOffset = 0
			r.currentSliceIndex++
			continue
		}

		remaining := len(buf) - totalRead
		remainingSlice := currentSliceLen - r.currentSliceOffset
		readSize := min(remaining, remainingSlice)
		copy(buf[totalRead:totalRead+readSize], currentSlice[r.currentSliceOffset:r.currentSliceOffset+readSize])
		r.currentSliceOffset += readSize
		totalRead += readSize
	}
	return totalRead, nil
}

// Length implements ResponseBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseBodyBuffer) Length() int {
	return int(C.envoy_dynamic_module_http_get_response_body_buffer_length(r.raw))
}

// Slice implements ResponseBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseBodyBuffer) Slices(iter func(view []byte)) {
	sliceCount := r.slicesCount()
	for i := 0; i < sliceCount; i++ {
		iter(r.sliceAt(i))
	}
}

func (r ResponseBodyBuffer) slicesCount() int {
	return int(C.envoy_dynamic_module_http_get_response_body_buffer_slices_count(r.raw))
}

func (r ResponseBodyBuffer) sliceAt(index int) []byte {
	var ptr *byte
	var size int
	C.envoy_dynamic_module_http_get_response_body_buffer_slice(r.raw,
		C.size_t(index),
		C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&ptr))),
		C.envoy_dynamic_module_type_DataSliceLengthResult(uintptr(unsafe.Pointer(&size))),
	)
	return unsafe.Slice(ptr, size)
}

// Copy implements ResponseBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseBodyBuffer) Copy() []byte {
	bytes := make([]byte, r.Length())
	offset := 0
	r.Slices(func(view []byte) {
		copy(bytes[offset:], view)
		offset += len(view)
	})
	return bytes
}

// ReadAt implements io.ReaderAt, and RequestBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseBodyBuffer) ReadAt(p []byte, off int64) (n int, err error) {
	length := r.Length()
	if off >= int64(length) {
		return 0, io.EOF
	}
	diff := int64(length) - off
	if int64(len(p)) > diff {
		p = p[:diff]
		err = io.EOF
	}
	C.envoy_dynamic_module_http_copy_out_response_body_buffer(
		r.raw, C.size_t(off), C.size_t(len(p)),
		C.envoy_dynamic_module_type_InModuleBufferPtr(uintptr(unsafe.Pointer(&p[0]))),
	)
	return len(p), err
}

// Append implements RequestBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r RequestBodyBuffer) Append(data []byte) {
	C.envoy_dynamic_module_http_append_request_body_buffer(
		r.raw,
		C.envoy_dynamic_module_type_InModuleBufferPtr(uintptr(unsafe.Pointer(&data[0]))),
		C.envoy_dynamic_module_type_InModuleBufferLength(len(data)),
	)
	runtime.KeepAlive(data)
}

// Prepend implements RequestBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r RequestBodyBuffer) Prepend(data []byte) {
	C.envoy_dynamic_module_http_prepend_request_body_buffer(
		r.raw,
		C.envoy_dynamic_module_type_InModuleBufferPtr(uintptr(unsafe.Pointer(&data[0]))),
		C.envoy_dynamic_module_type_InModuleBufferLength(len(data)),
	)
	runtime.KeepAlive(data)

}

// Drain implements RequestBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r RequestBodyBuffer) Drain(length int) {
	C.envoy_dynamic_module_http_drain_request_body_buffer(r.raw, C.size_t(length))
}

func (r RequestBodyBuffer) Replace(data []byte) {
	r.Drain(r.Length())
	r.Append(data)
}

// Append implements ResponseBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseBodyBuffer) Append(data []byte) {
	C.envoy_dynamic_module_http_append_response_body_buffer(
		r.raw,
		C.envoy_dynamic_module_type_InModuleBufferPtr(uintptr(unsafe.Pointer(&data[0]))),
		C.envoy_dynamic_module_type_InModuleBufferLength(len(data)),
	)
	runtime.KeepAlive(data)
}

// Prepend implements ResponseBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseBodyBuffer) Prepend(data []byte) {
	C.envoy_dynamic_module_http_prepend_response_body_buffer(
		r.raw,
		C.envoy_dynamic_module_type_InModuleBufferPtr(uintptr(unsafe.Pointer(&data[0]))),
		C.envoy_dynamic_module_type_InModuleBufferLength(len(data)),
	)
	runtime.KeepAlive(data)
}

// Drain implements ResponseBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseBodyBuffer) Drain(length int) {
	C.envoy_dynamic_module_http_drain_response_body_buffer(r.raw, C.size_t(length))
}

// Replace implements ResponseBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseBodyBuffer) Replace(data []byte) {
	r.Drain(r.Length())
	r.Append(data)
}

// Reader implements RequestBodyBuffer interface in abi_nocgo.go which is not included in the shared library.
func (r ResponseBodyBuffer) NewReader() io.Reader {
	return &responseBufferReader{buffer: r}
}

// responseBufferReader implements io.Reader for the ResponseBodyBuffer.
type responseBufferReader struct {
	buffer             ResponseBodyBuffer
	currentSliceIndex  int
	currentSliceOffset int
}

// Read implements io.Reader for the ResponseBodyBuffer.
func (r *responseBufferReader) Read(buf []byte) (int, error) {
	totalRead := 0
	slicesCount := r.buffer.slicesCount()
	// TODO: below obviously can be optimized. But for now, this is fine for the PoC.
	for totalRead < len(buf) {
		if r.currentSliceIndex >= slicesCount {
			return totalRead, io.EOF
		}
		currentSlice := r.buffer.sliceAt(r.currentSliceIndex)
		currentSliceLen := len(currentSlice)
		if r.currentSliceOffset >= currentSliceLen {
			r.currentSliceOffset = 0
			r.currentSliceIndex++
			continue
		}

		remaining := len(buf) - totalRead
		remainingSlice := currentSliceLen - r.currentSliceOffset
		readSize := min(remaining, remainingSlice)
		copy(buf[totalRead:totalRead+readSize], currentSlice[r.currentSliceOffset:r.currentSliceOffset+readSize])
		r.currentSliceOffset += readSize
		totalRead += readSize
	}
	return totalRead, nil
}

// HeaderValue represents a single header value whose data is owned by the Envoy.
//
// This is a view of the underlying data and doesn't copy the data.
//
// HeaderValue can be in any encoding, including non-UTF-8 encoding.
type HeaderValue struct {
	data *byte
	size int
}

// String implements HeaderValue interface in abi_nocgo.go which is not included in the shared library.
func (h HeaderValue) String() string {
	view := h.rawBytes()
	if !utf8.Valid(view) {
		panic("invalid utf-8 data. To handle non-utf-8 header value, use HeaderValue.Bytes function.")
	}
	return string(view)
}

// Bytes implements HeaderValue interface in abi_nocgo.go which is not included in the shared library.
func (h HeaderValue) Bytes() []byte {
	if h.data == nil {
		return nil
	}
	d := h.rawBytes()
	ret := make([]byte, h.size)
	copy(ret, d)
	return ret
}

func (h HeaderValue) rawBytes() []byte {
	if h.data == nil {
		return nil
	}
	return unsafe.Slice(h.data, h.size)
}

// Equal implements HeaderValue interface in abi_nocgo.go which is not included in the shared library.
func (h HeaderValue) Equal(str string) bool {
	if str == "" {
		return h.data == nil
	}
	target := unsafe.Slice(unsafe.StringData(str), len(str))
	runtime.KeepAlive(str)
	return bytes.Equal(h.rawBytes(), target)
}
