# Task: QOI to GIF with Cross-Platform Support
## Objective: Extend the QOI (Quite OK Image) library to support GIF creation from a folder of QOI images. 

-----------

### Requirements:

#### [QOI library](https://github.com/phoboslab/qoi) 
* [ ] Clone: https://github.com/phoboslab/qoi
* [ ] Cross-Platform Support: Make sure the project compiles and runs on Linux and Windows. You are to modify the project to use Waf build system: [Waf GitHub](https://github.com/ArduPilot/waf)
* [ ] Setup: Make sure the library works as expected by generating a couple of sample images. Hint: build the 'qoiconv' executable to be able to create QOI from PNG. 
* [ ] GIF Creation Extension: Implement functionality to generate an animated GIF from a folder of QOI images.
#### CLI: Create a CLI that:
* [ ] Accepts a path to the [YAML](https://github.com/fktn-k/fkYAML) config file, you can find file structure below.
* [ ] Generates the GIF according to the settings, use any library that you can find.
  * Be sure to handle the case when input images differ in sized with each other and desired output, you can either crop or upscale/downscale with the tools you'll already have as dependencies of QOI project. 
* [ ] Opens the resulting GIF with ffplay (if available).

------------

### Deliverables:

* [ ] Git Repository: Host the project on a Git repository (any service) and provide the URL. Note: The project may be a fork of the original project and CLI should be a part of it, thus your solution would consist of two projects: QOI conv used before and executable for the CLI demo, the QOI library itself is to be used as a header-only library. 
  - Properly set up `.gitignore` file.
  - Include a README.md with instructions on building, running, and testing the program.
  - In the dedicated folder include samples for testing including folders with images and configurations files (YAML).

##### The configuration file must follow this structure:

```yaml
input_folder: "path/to/qoi_images"  # Folder containing QOI images
output_gif: "output.gif"            # Output GIF file path
width:                              # Width of the output GIF
height:                              # Height of the output GIF
frame_delay: 100                     # Delay per frame in milliseconds
loop: true                           # Whether the GIF should loop
sort_order: "name"                   # Order of frames: "name" (default), "mtime" (modification time)
ffplay: true                         # Automatically open the GIF with ffplay if available
```

##### Command-Line Usage
Run the program with the following command:

```bash
./qoi_to_gif --config config.yaml
```
Where config.yaml is the path to your YAML configuration file.