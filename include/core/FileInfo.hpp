#pragma once

#include <string>
#include <filesystem>
#include <cstdint>

namespace sfi::core {

/**
 * @brief Represents metadata of a single indexed file.
 */
struct FileInfo {
    std::string filename;
    std::string path; // Full path
    std::string extension;
    uintmax_t size{0};
    uint64_t last_modified_time{0}; // Epoch time in seconds

    // Default constructor
    FileInfo() = default;

    // Parameterized constructor
    FileInfo(std::string name, std::string p, std::string ext, uintmax_t sz, uint64_t mod_time)
        : filename(std::move(name)), path(std::move(p)), extension(std::move(ext)), size(sz), last_modified_time(mod_time) {}
};

} // namespace sfi::core
