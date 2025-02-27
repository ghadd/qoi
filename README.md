# QOI Project

This project uses Waf as the build system to create a static library and executables.

## Prerequisites

- Python (version 3.x)
- Git

## Getting Started

1. Clone the repository and update submodules:

    ```sh
    git clone https://github.com/yourusername/qoi.git
    cd qoi
    git submodule update --init --recursive
    ```

2. Configure and build the project using Waf:

    ```sh
    python ./waf/waf-light configure build
    ```

3. Run the executables:

    ```sh
    ./build/qoi-conv.exe
    ./build/cli.exe
    ```

## Project Structure

- wscript: Waf build script.
- waf: Waf submodule.
- qoiconv.c, cli.cpp: Source files.
- qoi.h: Header file.
- fkYAML: YAML parsing library.
- gif-writer: Library for writing GIF files.
- stb: Collection of single-file public domain libraries for C/C++.

## Using qoi-conv

The `qoi-conv` executable can be used to convert between PNG and QOI formats. Run it with either a PNG or QOI file as input to generate the corresponding output format.

Example usage:

```sh
./build/qoi-conv.exe input.png output.qoi
./build/qoi-conv.exe input.qoi output.png
```

## Using cli.exe

The `cli.exe` executable provides a command-line interface for generating GIFs from a folder of QOI images. 

```
Usage: cli.exe --config config.yaml

Options:
  --config <path>  Path to the YAML configuration file.
```

### Configuration File Structure

The configuration file (`config.yaml`) must follow this structure:

```yaml
input_folder: "path/to/qoi_images"  # Folder containing QOI images
output_gif: "output.gif"            # Output GIF file path
width: 800                          # Width of the output GIF
height: 600                         # Height of the output GIF
frame_delay: 100                    # Delay per frame in milliseconds
loop: true                          # Whether the GIF should loop
sort_order: "name"                  # Order of frames: "name" (default), "mtime" (modification time)
ffplay: true                        # Automatically open the GIF with ffplay if available
```

### Command-Line Usage

Run the program with the following command:

```sh
./build/cli.exe --config config.yaml
```

Where `config.yaml` is the path to your YAML configuration file.