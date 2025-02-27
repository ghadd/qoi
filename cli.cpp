// Author: Daniil Timachov <daniiltimachov@gmail.com>

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>

#include "fkYAML/single_include/fkYAML/node.hpp"

#define QOI_IMPLEMENTATION
#include "qoi.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize2.h"

#include "gif-writer/gif.h"

namespace fs = std::filesystem;

struct Config {
    std::string input_folder;
    std::string output_gif;
    int frame_delay;
    bool loop;
    std::string sort_order;
    bool ffplay;
    int gif_width;
    int gif_height;
};

Config parse_config(const std::string& path) {
    // Load YAML from file
    std::ifstream ifs(path);
    auto node = fkyaml::node::deserialize(ifs);

    // Use get_value to extract values from the YAML node
    return {
        node["input_folder"].get_value<std::string>(),
        node["output_gif"].get_value<std::string>(),
        node["frame_delay"].get_value<int>(),
        node["loop"].get_value<bool>(),
        node["sort_order"].get_value<std::string>(),
        node["ffplay"].get_value<bool>(),
        node["gif_width"].get_value<int>(),
        node["gif_height"].get_value<int>()
    };
}

std::vector<fs::path> get_qoi_files(const std::string& folder, const std::string& sort_order) {
    std::vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.path().extension() == ".qoi") {
            files.push_back(entry.path());
        }
    }
    if (sort_order == "mtime") {
        std::sort(files.begin(), files.end(), [](const fs::path& a, const fs::path& b) {
            return fs::last_write_time(a) < fs::last_write_time(b);
            });
    }
    else {
        std::sort(files.begin(), files.end());
    }
    return files;
}

uint8_t* resize_frame(const void* data, int original_width, int original_height, int gif_width, int gif_height) {
    // Allocate memory for the resized image
    uint8_t* resized_data = (uint8_t*)malloc(gif_width * gif_height * 4); // Assuming 4 channels (RGBA)
    if (!resized_data) {
        std::cerr << "Failed to allocate memory for resized image" << std::endl;
        return nullptr;
    }

    // Perform the resizing operation using stb_image_resize (bilinear scaling)
    if (stbir_resize_uint8_srgb((const unsigned char*)data, original_width, original_height, 0, (unsigned char*)resized_data, gif_width, gif_height, 0, STBIR_RGBA)) {
        return resized_data;  // Return the resized image buffer
    }
    else {
        std::cerr << "Failed to resize image" << std::endl;
        free(resized_data);  // Free allocated memory in case of failure
        return nullptr;
    }
}

void create_gif(const Config& config, const std::vector<fs::path>& files) {
    GifWriter gif;
    if (!GifBegin(&gif, config.output_gif.c_str(), config.gif_width, config.gif_height, config.frame_delay, config.loop)) {
        std::cerr << "Failed to create GIF" << std::endl;
        return;
    }

    for (const auto& file : files) {
        qoi_desc desc;
        void* data = qoi_read(file.string().c_str(), &desc, 4);
        std::cout << "Read QOI: " << file << " " << desc.width << "x" << desc.height << " " << static_cast<int>(desc.channels) << " channels" << " " << (desc.colorspace == QOI_SRGB ? "RGB" : "Linear") << std::endl;

        if (!data) {
            std::cerr << "Failed to read QOI: " << file << std::endl;
            continue;
        }

        // Resize frame to fit the GIF dimensions
        uint8_t* resized_data = resize_frame(data, desc.width, desc.height, config.gif_width, config.gif_height);
        if (resized_data) {
            // Write resized frame to GIF
            if (!GifWriteFrame(&gif, resized_data, config.gif_width, config.gif_height, config.frame_delay, 8, true)) {
                std::cerr << "Failed to write frame" << std::endl;
                break;
            }
            free(resized_data);  // Free the resized buffer after use
        }
        else {
            std::cerr << "Failed to resize frame for " << file << std::endl;
        }

        free(data);
    }

    GifEnd(&gif);
}

void play_gif(const Config& config) {
    if (!config.ffplay) {
        return;
    }

    // Check if ffplay is installed on Linux/macOS or Windows
#if defined(__linux__) || defined(__APPLE__)
    if (system("which ffplay > /dev/null 2>&1") == 0) {
        std::string command = "ffplay -loop 0 " + config.output_gif + " &";
        system(command.c_str());
    }
    else {
        std::cerr << "ffplay is not installed. Unable to play GIF." << std::endl;
    }
#elif defined(_WIN32)
    // On Windows, use start command or ffmpeg
    if (system("where ffplay > NUL 2>&1") == 0) {
        std::string command = "ffplay -loop 0 " + config.output_gif + " &";
        system(command.c_str());
    }
    else {
        std::cerr << "ffplay is not installed. Trying to use default Windows viewer..." << std::endl;
        std::string command = "start " + config.output_gif;
        system(command.c_str());
    }
#else
    std::cerr << "Unsupported OS" << std::endl;
#endif
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config.yaml>" << std::endl;
        return 1;
    }

    std::cout << "QOI to GIF" << std::endl;
    std::cout << "Parsing config file: " << argv[1] << std::endl;

    Config config = parse_config(argv[1]);
    std::cout << "Input folder: " << config.input_folder << std::endl;
    std::cout << "Output GIF: " << config.output_gif << std::endl;
    std::cout << "Frame delay: " << config.frame_delay << std::endl;
    std::cout << "Loop: " << config.loop << std::endl;
    std::cout << "Sort order: " << config.sort_order << std::endl;
    std::cout << "ffplay: " << config.ffplay << std::endl;
    std::cout << "GIF Width: " << config.gif_width << std::endl;
    std::cout << "GIF Height: " << config.gif_height << std::endl;

    std::vector<fs::path> files = get_qoi_files(config.input_folder, config.sort_order);
    std::cout << "Found " << files.size() << " QOI images in " << config.input_folder << std::endl;
    for (auto&& file : files) {
        std::cout << file << std::endl;
    }

    if (files.empty()) {
        std::cerr << "No QOI images found in " << config.input_folder << std::endl;
        return 1;
    }

    create_gif(config, files);
    play_gif(config);
    return 0;
}
