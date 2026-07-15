#include "core/SearchEngine.hpp"
#include "utils/Utilities.hpp"
#include <shared_mutex>

namespace sfi::core {

SearchEngine::SearchEngine(std::shared_ptr<FileIndex> index) : m_index(std::move(index)) {}

std::vector<FileInfo> SearchEngine::search(const SearchQuery& query) const {
    std::vector<FileInfo> results;
    
    // Acquire a read lock on the index during the search
    std::shared_lock lock(m_index->getMutex());
    const auto& files = m_index->getFiles();

    for (const auto& file : files) {
        bool match = true;

        if (query.filename.has_value()) {
            if (!utils::StringUtils::containsIgnoreCase(file.filename, query.filename.value())) {
                match = false;
            }
        }
        
        if (match && query.extension.has_value()) {
            if (!utils::StringUtils::containsIgnoreCase(file.extension, query.extension.value())) {
                match = false;
            }
        }
        
        if (match && query.pathContains.has_value()) {
            if (!utils::StringUtils::containsIgnoreCase(file.path, query.pathContains.value())) {
                match = false;
            }
        }
        
        if (match && query.minSize.has_value()) {
            if (file.size < query.minSize.value()) {
                match = false;
            }
        }
        
        if (match && query.maxSize.has_value()) {
            if (file.size > query.maxSize.value()) {
                match = false;
            }
        }
        
        if (match) {
            results.push_back(file);
        }
    }
    
    return results;
}

} // namespace sfi::core
