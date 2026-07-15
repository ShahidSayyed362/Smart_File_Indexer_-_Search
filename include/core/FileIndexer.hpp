#pragma once
#include "core/FileIndex.hpp"
#include <string>
#include <memory>
#include <atomic>

namespace sfi::core {

class FileIndexer {
public:
    explicit FileIndexer(std::shared_ptr<FileIndex> index);

    // Recursively scans the directory and adds files to the index
    // Returns the number of files indexed in this run
    size_t indexDirectory(const std::string& directoryPath);
    
    // Allow cancellation
    void cancel();

private:
    std::shared_ptr<FileIndex> m_index;
    std::atomic<bool> m_cancelFlag{false};
};

} // namespace sfi::core
