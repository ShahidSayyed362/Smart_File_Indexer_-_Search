#include "io/Serializer.hpp"
#include "utils/Logger.hpp"
#include <fstream>
#include <iostream>
#include <shared_mutex>

namespace sfi::io {

Serializer::Serializer(std::shared_ptr<core::FileIndex> index) : m_index(std::move(index)) {}

// Helper function to write a string to binary stream (length followed by characters)
static void writeString(std::ofstream& out, const std::string& str) {
    size_t length = str.size();
    out.write(reinterpret_cast<const char*>(&length), sizeof(length));
    if (length > 0) {
        out.write(str.data(), length);
    }
}

// Helper function to read a string from binary stream
static std::string readString(std::ifstream& in) {
    size_t length = 0;
    in.read(reinterpret_cast<char*>(&length), sizeof(length));
    if (length == 0) return "";
    std::string str(length, '\0');
    in.read(str.data(), length);
    return str;
}

bool Serializer::saveToFile(const std::string& filepath) const {
    std::ofstream out(filepath, std::ios::binary);
    if (!out.is_open()) {
        utils::Logger::getInstance().error("Failed to open file for writing: " + filepath);
        return false;
    }

    std::shared_lock lock(m_index->getMutex());
    const auto& files = m_index->getFiles();

    // Write a simple header/magic number
    const uint32_t magic = 0x5F494E44; // "_IND"
    out.write(reinterpret_cast<const char*>(&magic), sizeof(magic));

    size_t count = files.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& file : files) {
        writeString(out, file.filename);
        writeString(out, file.path);
        writeString(out, file.extension);
        out.write(reinterpret_cast<const char*>(&file.size), sizeof(file.size));
        out.write(reinterpret_cast<const char*>(&file.last_modified_time), sizeof(file.last_modified_time));
    }

    utils::Logger::getInstance().info("Saved " + std::to_string(count) + " files to index file: " + filepath);
    return true;
}

bool Serializer::loadFromFile(const std::string& filepath) {
    std::ifstream in(filepath, std::ios::binary);
    if (!in.is_open()) {
        utils::Logger::getInstance().error("Failed to open index file for reading: " + filepath);
        return false;
    }

    uint32_t magic = 0;
    in.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    if (magic != 0x5F494E44) {
        utils::Logger::getInstance().error("Invalid index file format.");
        return false;
    }

    size_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    m_index->clear();
    m_index->reserve(count);

    for (size_t i = 0; i < count; ++i) {
        if (!in.good()) {
            utils::Logger::getInstance().error("Error reading index file prematurely.");
            return false;
        }

        core::FileInfo info;
        info.filename = readString(in);
        info.path = readString(in);
        info.extension = readString(in);
        in.read(reinterpret_cast<char*>(&info.size), sizeof(info.size));
        in.read(reinterpret_cast<char*>(&info.last_modified_time), sizeof(info.last_modified_time));

        m_index->addFile(std::move(info));
    }

    utils::Logger::getInstance().info("Loaded " + std::to_string(count) + " files from index file: " + filepath);
    return true;
}

} // namespace sfi::io
