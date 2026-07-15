#pragma once
#include "core/FileIndex.hpp"
#include "core/FileIndexer.hpp"
#include "core/SearchEngine.hpp"
#include "io/Serializer.hpp"
#include <memory>
#include <string>

namespace sfi::app {

class Application {
public:
    Application();
    ~Application() = default;

    void run(int argc, char* argv[]);

private:
    void printHelp() const;
    void handleInteractiveLoop();
    void processCommand(const std::string& input);

    std::shared_ptr<core::FileIndex> m_index;
    std::unique_ptr<core::FileIndexer> m_indexer;
    std::unique_ptr<core::SearchEngine> m_searchEngine;
    std::unique_ptr<io::Serializer> m_serializer;
    
    std::string m_indexFilePath;
};

} // namespace sfi::app
