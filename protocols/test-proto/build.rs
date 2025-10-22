// Build script to generate Rust code from protobuf files
// This runs during `cargo build` before compiling the main crate

fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Compile the proto file
    prost_build::compile_protos(&["test.proto"], &["./"])?;

    Ok(())
}
