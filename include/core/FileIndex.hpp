#pragma once

#include "core/FileInfo.hpp"
#include <vector>
#include <shared_mutex>

namespace sfi::core {

class FileIndex {
public:
    FileIndex() = default;
    ~FileIndex() = default;

    // Disable copy
    FileIndex(const FileIndex&) = delete;
    FileIndex& operator=(const FileIndex&) = delete;

    void addFile(const FileInfo& info);
    void addFile(FileInfo&& info);
    void clear();
    
    size_t getFileCount() const;
    const std::vector<FileInfo>& getFiles() const;
    std::shared_mutex& getMutex() const;
    
    void reserve(size_t size);
    
private:
    mutable std::shared_mutex m_mutex;
    std::vector<FileInfo> m_files;
};

} // namespace sfi::core
