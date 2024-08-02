//go:build !cgo

package envoy

import "io"

// This file is only included when cgo is disabled which is used for testing purposes.

// EnvoyFilterInstance is an opaque object that represents the underlying Envoy Http filter instance.
// This is used to interact with it from the module code.
type EnvoyFilterInstance interface {
	// GetRequestBodyBuffer returns the entire request body buffer that is currently buffered.
	GetRequestBodyBuffer() RequestBodyBuffer
	// GetResponseBodyBuffer returns the entire response body buffer that is currently buffered.
	GetResponseBodyBuffer() ResponseBodyBuffer
	// ContinueRequest is a function that continues the request processing.
	ContinueRequest()
	// ContinueResponse is a function that continues the response processing.
	ContinueResponse()
	// SendResponse is a function that sends the response to the downstream.
	SendResponse(statusCode int, headers [][2]string, body []byte)
}

// RequestHeaders is an opaque object that represents the underlying Envoy Http request headers map.
// This is used to interact with it from the module code.
type RequestHeaders interface {
	// Get returns the first header value for the given key. To handle multiple values, use the Values method.
	// Returns true at the second return value if the key exists.
	Get(key string) (HeaderValue, bool)
	// Values iterates over the header values for the given key.
	Values(key string, iter func(value HeaderValue))
	// Set sets the value for the given key. If multiple values are set for the same key,
	// this removes all the previous values and sets the new single value.
	Set(key, value string)
	// Remove removes the value for the given key. If multiple values are set for the same key,
	// this removes all the values.
	Remove(key string)
}

// ResponseHeaders is an opaque object that represents the underlying Envoy Http response headers map.
// This is used to interact with it from the module code.
type ResponseHeaders interface {
	// Get returns the first header value for the given key. To handle multiple values, use the Values method.
	// Returns true at the second return value if the key exists.
	Get(key string) (HeaderValue, bool)
	// Values iterates over the header values for the given key.
	Values(key string, iter func(value HeaderValue))
	// Set sets the value for the given key. If multiple values are set for the same key,
	// this removes all the previous values and sets the new single value.
	Set(key, value string)
	// Remove removes the value for the given key. If multiple values are set for the same key,
	// this removes all the values.
	Remove(key string)
}

// RequestBodyBuffer is an opaque object that represents the underlying Envoy Http request body buffer.
// This is used to interact with it from the module code. A buffer consists of a multiple slices of data,
// not a single contiguous buffer.
//
// This provides a zero-copy view of the HTTP request body buffer.
//
// This implements io.ReaderAt interface.
type RequestBodyBuffer interface {
	io.ReaderAt

	// Length returns the total number of bytes in the buffer.
	Length() int
	// Slices iterates over the slices of the buffer. The view byte slice must NOT be saved as the
	// memory is owned by the Envoy. To take a copy of the buffer, use the Copy method.
	Slices(iter func(view []byte))
	// Copy returns a copy of the bytes in the buffer as a single contiguous buffer.
	Copy() []byte
	// Append appends the data to the buffer.
	Append(data []byte)
	// Prepend prepends the data to the buffer.
	Prepend(data []byte)
	// Drain removes the given number of bytes from the front of the buffer.
	Drain(length int)
	// Replace replaces the buffer with the given data. This doesn't take the ownership of the data.
	// Therefore, data will be copied to the buffer internally.
	Replace(data []byte)
	// NewReader returns an io.Reader for the buffer.
	NewReader() io.Reader
}

// ResponseBodyBuffer is an opaque object that represents the underlying Envoy Http response body buffer.
// This is used to interact with it from the module code. A buffer consists of a multiple slices of data,
// not a single contiguous buffer.
//
// This provides a zero-copy view of the HTTP response body buffer.
//
// This implements io.ReaderAt interface.
type ResponseBodyBuffer interface {
	io.ReaderAt

	// Length returns the total number of bytes in the buffer.
	Length() int
	// Slices iterates over the slices of the buffer. The view byte slice must NOT be saved as the
	// memory is owned by the Envoy. To take a copy of the buffer, use the Copy method.
	Slices(iter func(view []byte))
	// Copy returns a copy of the bytes in the buffer as a single contiguous buffer.
	Copy() []byte
	// Append appends the data to the buffer.
	Append(data []byte)
	// Prepend prepends the data to the buffer.
	Prepend(data []byte)
	// Drain removes the given number of bytes from the front of the buffer.
	Drain(length int)
	// Replace replaces the buffer with the given data. This doesn't take the ownership of the data.
	// Therefore, data will be copied to the buffer internally.
	Replace(data []byte)
	// NewReader returns an io.Reader for the buffer.
	NewReader() io.Reader
}

// HeaderValue represents a single header value whose data is owned by the Envoy.
//
// This is a view of the underlying data and doesn't copy the data.
//
// HeaderValue can be in any encoding, including non-UTF-8 encoding.
type HeaderValue interface {
	// String returns the string representation of the header value.
	// This copies the underlying data to a new buffer and returns the string.
	//
	// Note: The header value might not be UTF-8 encoded. Therefore,
	// this checks if the data is valid UTF-8 and returns the string.
	// To get the raw data, use the HeaderValue.Bytes function.
	String() string

	// Bytes returns the copied data of the header value.
	Bytes() []byte

	// Equal returns true if the header value is equal to the given string.
	//
	// This doesn't copy the data and compares the data directly.
	Equal(str string) bool
}
