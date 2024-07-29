use std::{
    io::Read,
    sync::{Arc, Mutex},
};

use envoy_dynamic_modules_rust_sdk::*;

init!(new_http_filter);

/// new_http_filter is the entry point for the filter chains.
///
/// This function is called by the Envoy corresponding to the filter chain configuration.
fn new_http_filter(config: &str) -> Box<dyn HttpFilter> {
    // Each filter is written in a way that it passes the conformance tests.
    match config {
        "helloworld" => Box::new(HelloWorldFilter {}),
        "delay" => Box::new(DelayFilter {
            atomic: std::sync::atomic::AtomicUsize::new(1),
        }),
        "headers" => Box::new(HeadersFilter {}),
        "bodies" => Box::new(BodiesFilter {}),
        "bodies_replace" => Box::new(BodiesReplace {}),
        "send_response" => Box::new(SendResponseFilter {}),
        _ => panic!("Unknown config: {}", config),
    }
}

/// HelloWorldFilter is a simple filter that prints a message for each filter call.
///
/// This implements the [`HttpFilter`] trait, and will be craeted per each filter chain.
struct HelloWorldFilter {}

impl HttpFilter for HelloWorldFilter {
    fn new_instance(
        &mut self,
        _envoy_filter_instance: EnvoyFilterInstance,
    ) -> Box<dyn HttpFilterInstance> {
        Box::new(HelloWorldFilterInstance {})
    }

    fn destroy(&self) {
        println!("HelloWorldFilter destroyed");
    }
}

/// HelloWorldFilterInstance is a simple filter instance that prints a message for each filter call.
///
/// This implements the [`HttpFilterInstance`] trait, and will be created per each request.
struct HelloWorldFilterInstance {}

impl HttpFilterInstance for HelloWorldFilterInstance {
    fn request_headers(
        &mut self,
        _request_headers: &RequestHeaders,
        _end_of_stream: bool,
    ) -> RequestHeadersStatus {
        println!("RequestHeaders called");
        RequestHeadersStatus::Continue
    }

    fn request_body(
        &mut self,
        _request_body_frame: &RequestBodyBuffer,
        _end_of_stream: bool,
    ) -> RequestBodyStatus {
        println!("RequestBody called");
        RequestBodyStatus::Continue
    }

    fn response_headers(
        &mut self,
        _response_headers: &ResponseHeaders,
        _end_of_stream: bool,
    ) -> ResponseHeadersStatus {
        println!("ResponseHeaders called");
        ResponseHeadersStatus::Continue
    }

    fn response_body(
        &mut self,
        _response_body_frame: &ResponseBodyBuffer,
        _end_of_stream: bool,
    ) -> ResponseBodyStatus {
        println!("ResponseBody called");
        ResponseBodyStatus::Continue
    }

    fn destroy(&mut self) {
        println!("Destroy called");
    }
}

/// HeadersFilter is a filter that manipulates headers.
///
/// This implements the [`HttpFilter`] trait, and will be created per each filter chain.
struct HeadersFilter {}

impl HttpFilter for HeadersFilter {
    fn new_instance(
        &mut self,
        _envoy_filter_instance: EnvoyFilterInstance,
    ) -> Box<dyn HttpFilterInstance> {
        Box::new(HeadersFilterInstance {})
    }
}

/// HeadersFilterInstance is a filter instance that manipulates headers.
///
/// This implements the [`HttpFilterInstance`] trait, and will be created per each request.
struct HeadersFilterInstance {}

impl HttpFilterInstance for HeadersFilterInstance {
    fn request_headers(
        &mut self,
        request_headers: &RequestHeaders,
        _end_of_stream: bool,
    ) -> RequestHeadersStatus {
        if let Some(value) = request_headers.get("foo") {
            if value != "value" {
                panic!("expected this-is to be \"value\", got {:?}", value);
            } else {
                println!("foo: {}", value);
            }
        }

        request_headers
            .values("multiple-values")
            .iter()
            .for_each(|value| {
                println!("multiple-values: {}", value);
            });

        request_headers.remove("multiple-values");
        request_headers.set("foo", "yes");
        request_headers.set("multiple-values-to-be-single", "single");
        RequestHeadersStatus::Continue
    }

    fn response_headers(
        &mut self,
        response_headers: &ResponseHeaders,
        _end_of_stream: bool,
    ) -> ResponseHeadersStatus {
        if let Some(value) = response_headers.get("this-is") {
            if value != "response-header" {
                panic!(
                    "expected this-is to be \"response-header\", got {:?}",
                    value
                );
            } else {
                println!("this-is: {}", value);
            }
        }

        response_headers
            .values("this-is-2")
            .iter()
            .for_each(|value| {
                println!("this-is-2: {}", value);
            });

        response_headers.remove("this-is-2");
        response_headers.set("this-is", "response-header");
        response_headers.set("multiple-values-res-to-be-single", "single");

        ResponseHeadersStatus::Continue
    }
}

/// DelayFilter is a filter that delays the request.
///
/// This implements the [`HttpFilter`] trait, and will be created per each filter chain.
struct DelayFilter {
    atomic: std::sync::atomic::AtomicUsize,
}

impl HttpFilter for DelayFilter {
    fn new_instance(
        &mut self,
        envoy_filter_instance: EnvoyFilterInstance,
    ) -> Box<dyn HttpFilterInstance> {
        let req_no = self
            .atomic
            .fetch_add(1, std::sync::atomic::Ordering::SeqCst);

        let envoy_filter_instance = Arc::new(Mutex::new(Some(envoy_filter_instance)));
        Box::new(DelayFilterInstance {
            req_no,
            envoy_filter_instance,
        })
    }
}

/// DelayFilterInstance is a filter instance that delays the request.
///
/// This implements the [`HttpFilterInstance`] trait, and will be created per each request.
struct DelayFilterInstance {
    req_no: usize,
    envoy_filter_instance: Arc<Mutex<Option<EnvoyFilterInstance>>>,
}

impl HttpFilterInstance for DelayFilterInstance {
    fn request_headers(
        &mut self,
        _request_headers: &RequestHeaders,
        _end_of_stream: bool,
    ) -> RequestHeadersStatus {
        if self.req_no == 1 {
            let envoy_filter_instance = self.envoy_filter_instance.clone();
            let req_no = self.req_no;
            std::thread::spawn(move || {
                println!("blocking for 1 second at RequestHeaders with id {}", req_no);
                std::thread::sleep(std::time::Duration::from_secs(1));
                println!("calling ContinueRequest with id {}", req_no);
                if let Some(envoy_filter_instance) = envoy_filter_instance.lock().unwrap().as_ref()
                {
                    envoy_filter_instance.continue_request();
                }
            });
            println!(
                "RequestHeaders returning StopAllIterationAndBuffer with id {}",
                self.req_no
            );
            RequestHeadersStatus::StopAllIterationAndBuffer
        } else {
            println!("RequestHeaders called with id {}", self.req_no);
            RequestHeadersStatus::Continue
        }
    }

    fn request_body(
        &mut self,
        _request_body_frame: &RequestBodyBuffer,
        _end_of_stream: bool,
    ) -> RequestBodyStatus {
        if self.req_no == 2 {
            let envoy_filter_instance = self.envoy_filter_instance.clone();
            let req_no = self.req_no;
            std::thread::spawn(move || {
                println!("blocking for 1 second at RequestBody with id {}", req_no);
                std::thread::sleep(std::time::Duration::from_secs(1));
                println!("calling ContinueRequest with id {}", req_no);
                if let Some(envoy_filter_instance) = envoy_filter_instance.lock().unwrap().as_ref()
                {
                    envoy_filter_instance.continue_request();
                }
            });
            println!(
                "RequestBody returning StopIterationAndBuffer with id {}",
                self.req_no
            );
            RequestBodyStatus::StopIterationAndBuffer
        } else {
            println!("RequestBody called with id {}", self.req_no);
            RequestBodyStatus::Continue
        }
    }

    fn response_headers(
        &mut self,
        _response_headers: &ResponseHeaders,
        _end_of_stream: bool,
    ) -> ResponseHeadersStatus {
        if self.req_no == 3 {
            let envoy_filter_instance = self.envoy_filter_instance.clone();
            let req_no = self.req_no;
            std::thread::spawn(move || {
                println!(
                    "blocking for 1 second at ResponseHeaders with id {}",
                    req_no
                );
                std::thread::sleep(std::time::Duration::from_secs(1));
                println!("calling ContinueResponse with id {}", req_no);
                if let Some(envoy_filter_instance) = envoy_filter_instance.lock().unwrap().as_ref()
                {
                    envoy_filter_instance.continue_response();
                }
            });
            println!(
                "ResponseHeaders returning StopAllIterationAndBuffer with id {}",
                self.req_no
            );

            ResponseHeadersStatus::StopAllIterationAndBuffer
        } else {
            println!("ResponseHeaders called with id {}", self.req_no);
            ResponseHeadersStatus::Continue
        }
    }

    fn response_body(
        &mut self,
        _response_body_frame: &ResponseBodyBuffer,
        _end_of_stream: bool,
    ) -> ResponseBodyStatus {
        if self.req_no == 4 {
            let envoy_filter_instance = self.envoy_filter_instance.clone();
            let req_no = self.req_no;
            std::thread::spawn(move || {
                println!("blocking for 1 second at ResponseBody with id {}", req_no);
                std::thread::sleep(std::time::Duration::from_secs(1));
                println!("calling ContinueResponse with id {}", req_no);
                if let Some(envoy_filter_instance) = envoy_filter_instance.lock().unwrap().as_ref()
                {
                    envoy_filter_instance.continue_response();
                }
            });
            println!(
                "ResponseBody returning StopIterationAndBuffer with id {}",
                self.req_no
            );

            ResponseBodyStatus::StopIterationAndBuffer
        } else {
            println!("ResponseBody called with id {}", self.req_no);
            ResponseBodyStatus::Continue
        }
    }

    fn destroy(&mut self) {
        *self.envoy_filter_instance.lock().unwrap() = None;
    }
}

/// BodyFilter is a filter that manipulates request/response bodies.
///
/// This implements the [`HttpFilter`] trait, and will be created per each filter chain.
struct BodiesFilter {}

impl HttpFilter for BodiesFilter {
    fn new_instance(
        &mut self,
        envoy_filter_instance: EnvoyFilterInstance,
    ) -> Box<dyn HttpFilterInstance> {
        Box::new(BodiesFilterInstance {
            envoy_filter_instance,
        })
    }
}

/// BodiesFilterInstance is a filter instance that manipulates request/response bodies.
///
/// This implements the [`HttpFilterInstance`] trait, and will be created per each request.
struct BodiesFilterInstance {
    envoy_filter_instance: EnvoyFilterInstance,
}

impl HttpFilterInstance for BodiesFilterInstance {
    fn request_body(
        &mut self,
        request_body_frame: &RequestBodyBuffer,
        end_of_stream: bool,
    ) -> RequestBodyStatus {
        println!(
            "new request body frame: {}",
            String::from_utf8(request_body_frame.copy()).unwrap()
        );
        if !end_of_stream {
            // Wait for the end of the stream to see the full body.
            return RequestBodyStatus::StopIterationAndBuffer;
        }

        // Get the entire request body reference - this does not copy the body.
        let entire_body = self.envoy_filter_instance.get_request_body_buffer();
        println!(
            "entire request body: {}",
            String::from_utf8(entire_body.copy()).unwrap()
        );

        // This demonstrates how to use the reader to read the body.
        let mut reader = entire_body.reader();
        let mut buf = vec![0; 2];
        let mut offset = 0;
        loop {
            let n = reader.read(&mut buf).unwrap();
            if n == 0 {
                break;
            }
            println!(
                "request body read 2 bytes offset at {}: \"{}\"",
                offset,
                std::str::from_utf8(&buf[..n]).unwrap()
            );
            offset += 2;
        }

        // Replace the entire body with 'Y' without copying.
        for i in entire_body.slices() {
            for j in i {
                *j = b'X';
            }
        }
        RequestBodyStatus::Continue
    }

    fn response_body(
        &mut self,
        response_body_frame: &ResponseBodyBuffer,
        end_of_stream: bool,
    ) -> ResponseBodyStatus {
        println!(
            "new response body frame: {}",
            String::from_utf8(response_body_frame.copy()).unwrap()
        );
        if !end_of_stream {
            // Wait for the end of the stream to see the full body.
            return ResponseBodyStatus::StopIterationAndBuffer;
        }

        // Get the entire response body reference - this does not copy the body.
        let entire_body = self.envoy_filter_instance.get_response_body_buffer();
        println!(
            "entire response body: {}",
            String::from_utf8(entire_body.copy()).unwrap()
        );

        // This demonstrates how to use the reader to read the body.
        let mut reader = entire_body.reader();
        let mut buf = vec![0; 2];
        let mut offset = 0;
        loop {
            let n = reader.read(&mut buf).unwrap();
            if n == 0 {
                break;
            }
            println!(
                "response body read 2 bytes offset at {}: \"{}\"",
                offset,
                std::str::from_utf8(&buf[..n]).unwrap()
            );
            offset += 2;
        }

        // Replace the entire body with 'Y' without copying.
        for i in entire_body.slices() {
            for j in i {
                *j = b'Y';
            }
        }

        ResponseBodyStatus::Continue
    }
}

/// BodiesReplaceFilter is a filter that replaces request/response bodies.
///
/// This implements the [`HttpFilter`] trait, and will be created per each filter chain.
struct BodiesReplace {}

impl HttpFilter for BodiesReplace {
    fn new_instance(
        &mut self,
        envoy_filter_instance: EnvoyFilterInstance,
    ) -> Box<dyn HttpFilterInstance> {
        Box::new(BodiesReplaceInstance {
            envoy_filter_instance,
            request_append: String::new(),
            request_prepend: String::new(),
            request_replace: String::new(),
            response_append: String::new(),
            response_prepend: String::new(),
            response_replace: String::new(),
        })
    }
}

/// BodiesReplaceInstance is a filter instance that replaces request/response bodies.
///
/// This implements the [`HttpFilterInstance`] trait, and will be created per each request.
struct BodiesReplaceInstance {
    envoy_filter_instance: EnvoyFilterInstance,
    request_append: String,
    request_prepend: String,
    request_replace: String,
    response_append: String,
    response_prepend: String,
    response_replace: String,
}

impl HttpFilterInstance for BodiesReplaceInstance {
    fn request_headers(
        &mut self,
        request_headers: &RequestHeaders,
        _end_of_stream: bool,
    ) -> RequestHeadersStatus {
        if let Some(value) = request_headers.get("append") {
            self.request_append = value.to_string();
        }
        if let Some(value) = request_headers.get("prepend") {
            self.request_prepend = value.to_string();
        }
        if let Some(value) = request_headers.get("replace") {
            self.request_replace = value.to_string();
        }
        request_headers.remove("content-length"); // Remove content-length header to avoid mismatch.
        RequestHeadersStatus::Continue
    }

    fn request_body(
        &mut self,
        _request_body_frame: &RequestBodyBuffer,
        end_of_stream: bool,
    ) -> RequestBodyStatus {
        if !end_of_stream {
            // Wait for the end of the stream to see the full body.
            return RequestBodyStatus::StopIterationAndBuffer;
        }

        let entire_body = self.envoy_filter_instance.get_request_body_buffer();
        if !self.request_append.is_empty() {
            entire_body.append(self.request_append.as_bytes());
        }
        if !self.request_prepend.is_empty() {
            entire_body.prepend(self.request_prepend.as_bytes());
        }
        if !self.request_replace.is_empty() {
            entire_body.replace(self.request_replace.as_bytes());
        }
        RequestBodyStatus::Continue
    }

    fn response_headers(
        &mut self,
        response_headers: &ResponseHeaders,
        _end_of_stream: bool,
    ) -> ResponseHeadersStatus {
        if let Some(value) = response_headers.get("append") {
            self.response_append = value.to_string();
        }
        if let Some(value) = response_headers.get("prepend") {
            self.response_prepend = value.to_string();
        }
        if let Some(value) = response_headers.get("replace") {
            self.response_replace = value.to_string();
        }
        response_headers.remove("content-length"); // Remove content-length header to avoid mismatch.
        ResponseHeadersStatus::Continue
    }

    fn response_body(
        &mut self,
        _response_body_frame: &ResponseBodyBuffer,
        end_of_stream: bool,
    ) -> ResponseBodyStatus {
        if !end_of_stream {
            // Wait for the end of the stream to see the full body.
            return ResponseBodyStatus::StopIterationAndBuffer;
        }

        let entire_body = self.envoy_filter_instance.get_response_body_buffer();
        if !self.response_append.is_empty() {
            entire_body.append(self.response_append.as_bytes());
        }
        if !self.response_prepend.is_empty() {
            entire_body.prepend(self.response_prepend.as_bytes());
        }
        if !self.response_replace.is_empty() {
            entire_body.replace(self.response_replace.as_bytes());
        }
        ResponseBodyStatus::Continue
    }
}

/// SendResponseFilter is a filter that sends a response.
///
/// This implements the [`HttpFilter`] trait, and will be created per each filter chain.
struct SendResponseFilter {}

impl HttpFilter for SendResponseFilter {
    fn new_instance(
        &mut self,
        envoy_filter_instance: EnvoyFilterInstance,
    ) -> Box<dyn HttpFilterInstance> {
        Box::new(SendResponseFilterInstance {
            envoy_filter_instance,
            on_response_headers: false,
        })
    }
}

/// SendResponseFilterInstance is a filter instance that sends a response.
///
/// This implements the [`HttpFilterInstance`] trait, and will be created per each request.
struct SendResponseFilterInstance {
    envoy_filter_instance: EnvoyFilterInstance,
    on_response_headers: bool,
}

impl HttpFilterInstance for SendResponseFilterInstance {
    fn request_headers(
        &mut self,
        request_headers: &RequestHeaders,
        _end_of_stream: bool,
    ) -> RequestHeadersStatus {
        if let Some(value) = request_headers.get(":path") {
            if value == "/on_request" {
                let headers: Vec<(&[u8], &[u8])> = vec![
                    ("foo".as_bytes(), "bar".as_bytes()),
                    ("bar".as_bytes(), "baz".as_bytes()),
                ];
                self.envoy_filter_instance.send_response(
                    401,
                    headers.as_slice(),
                    "local response at request headers".as_bytes(),
                );
            }
            if value == "/on_response" {
                self.on_response_headers = true;
            }
        }
        RequestHeadersStatus::Continue
    }

    fn response_headers(
        &mut self,
        _response_headers: &ResponseHeaders,
        _end_of_stream: bool,
    ) -> ResponseHeadersStatus {
        if self.on_response_headers {
            let headers: Vec<(&[u8], &[u8])> = vec![("dog".as_bytes(), "cat".as_bytes())];
            self.envoy_filter_instance.send_response(
                500,
                headers.as_slice(),
                "local response at response headers".as_bytes(),
            );
        }
        ResponseHeadersStatus::Continue
    }
}
