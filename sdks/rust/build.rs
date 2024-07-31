use std::env;
use std::path::PathBuf;

fn main() {
    let path = match env::var("ABI_HEADER_ABSOLUTE_PATH") {
        Ok(val) => val,
        Err(_) => String::from("src/abi.h"),
    };
    let bindings = bindgen::Builder::default()
        .header(path)
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
