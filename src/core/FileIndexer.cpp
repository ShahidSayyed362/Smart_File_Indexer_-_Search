#include "core/FileIndexer.hpp"
#include "utils/Logger.hpp"
#include <filesystem>
#include <system_error>
#include <chrono>

namespace fs = std::filesystem;

namespace sfi::core {

FileIndexer::FileIndexer(std::shared_ptr<FileIndex> index) : m_index(std::move(index)) {}

void FileIndexer::cancel() {
    m_cancelFlag = true;
}

size_t FileIndexer::indexDirectory(const std::string& directoryPath) {
    m_cancelFlag = false;
    size_t count = 0;
    
    fs::path pathToIndex(directoryPath);
    
    if (!fs::exists(pathToIndex) || !fs::is_directory(pathToIndex)) {
        utils::Logger::getInstance().error("Invalid directory path: " + directoryPath);
        return 0;
    }
    
    utils::Logger::getInstance().info("Starting indexing of directory: " + directoryPath);
    
    auto options = fs::directory_options::skip_permission_denied;
    
    std::error_code ec;
    for (auto it = fs::recursive_directory_iterator(pathToIndex, options, ec);
         it != fs::recursive_directory_iterator();
         it.increment(ec)) {
         
        if (m_cancelFlag) {
            utils::Logger::getInstance().info("Indexing cancelled by user.");
            break;
        }

        if (ec) {
            utils::Logger::getInstance().warn("Error accessing path: " + ec.message());
            continue;
        }

        if (it->is_regular_file(ec)) {
            try {
                auto filepath = it->path();
                FileInfo info;
                info.filename = filepath.filename().string();
                info.path = filepath.string();
                info.extension = filepath.extension().string();
                info.size = fs::file_size(filepath, ec);
                
                auto ftime = fs::last_write_time(filepath, ec);
                
                // In C++20, we can use clock_cast if needed, or simple duration casting
                // Note: std::chrono::clock_cast is C++20 but some compilers might not support it fully yet.
                // An easier way is just getting the epoch directly if it's based on system_clock.
                // Some STL implementations map file_clock to system_clock directly.
                auto sys_time = std::chrono::file_clock::to_sys(ftime);
                info.last_modified_time = std::chrono::duration_cast<std::chrono::seconds>(sys_time.time_since_epoch()).count();
                
                m_index->addFile(std::move(info));
                count++;
            } catch (const std::exception& ex) {
                utils::Logger::getInstance().warn("Exception while indexing file: " + std::string(ex.what()));
            }
        }
    }
    
    utils::Logger::getInstance().info("Indexing complete. Files indexed: " + std::to_string(count));
    return count;
}

} // namespace sfi::core
