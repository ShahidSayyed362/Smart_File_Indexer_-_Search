#pragma once
#include "core/FileIndex.hpp"
#include <vector>
#include <string>
#include <memory>
#include <optional>

namespace sfi::core {

struct SearchQuery {
    std::optional<std::string> filename;
    std::optional<std::string> extension;
    std::optional<std::string> pathContains;
    std::optional<uintmax_t> minSize;
    std::optional<uintmax_t> maxSize;
};

class SearchEngine {
public:
    explicit SearchEngine(std::shared_ptr<FileIndex> index);

    std::vector<FileInfo> search(const SearchQuery& query) const;

private:
    std::shared_ptr<FileIndex> m_index;
};

} // namespace sfi::core
