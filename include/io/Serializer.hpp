#pragma once
#include "core/FileIndex.hpp"
#include <string>
#include <memory>

namespace sfi::io {

class Serializer {
public:
    explicit Serializer(std::shared_ptr<core::FileIndex> index);

    bool saveToFile(const std::string& filepath) const;
    bool loadFromFile(const std::string& filepath);

private:
    std::shared_ptr<core::FileIndex> m_index;
};

} // namespace sfi::io
