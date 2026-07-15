#include "core/FileIndex.hpp"
#include <mutex>

namespace sfi::core {

void FileIndex::addFile(const FileInfo& info) {
    std::unique_lock lock(m_mutex);
    m_files.push_back(info);
}

void FileIndex::addFile(FileInfo&& info) {
    std::unique_lock lock(m_mutex);
    m_files.push_back(std::move(info));
}

void FileIndex::clear() {
    std::unique_lock lock(m_mutex);
    m_files.clear();
}

size_t FileIndex::getFileCount() const {
    std::shared_lock lock(m_mutex);
    return m_files.size();
}

const std::vector<FileInfo>& FileIndex::getFiles() const {
    return m_files;
}

std::shared_mutex& FileIndex::getMutex() const {
    return m_mutex;
}

void FileIndex::reserve(size_t size) {
    std::unique_lock lock(m_mutex);
    m_files.reserve(size);
}

} // namespace sfi::core
