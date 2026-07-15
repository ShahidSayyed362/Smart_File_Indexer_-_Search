#include "app/Application.hpp"
#include "utils/Logger.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

namespace sfi::app {

Application::Application() 
    : m_index(std::make_shared<core::FileIndex>()),
      m_indexer(std::make_unique<core::FileIndexer>(m_index)),
      m_searchEngine(std::make_unique<core::SearchEngine>(m_index)),
      m_serializer(std::make_unique<io::Serializer>(m_index)),
      m_indexFilePath("sfi_index.bin") {
}

void Application::run(int argc, char* argv[]) {
    utils::Logger::getInstance().setLogFile("sfi.log");
    utils::Logger::getInstance().info("Smart File Indexer Started.");
    
    // Try to load existing index
    if (m_serializer->loadFromFile(m_indexFilePath)) {
        std::cout << "Loaded existing index with " << m_index->getFileCount() << " files.\n";
    } else {
        std::cout << "No existing index found. Please index a directory first.\n";
    }

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            printHelp();
            return;
        }
    } else {
        handleInteractiveLoop();
    }
}

void Application::printHelp() const {
    std::cout << "Smart File Indexer Commands:\n"
              << "  index <path>              - Index a directory recursively\n"
              << "  search name <name>        - Search by filename\n"
              << "  search ext <extension>    - Search by extension\n"
              << "  search path <path_part>   - Search by path containing string\n"
              << "  search size <min> <max>   - Search by file size range (in bytes)\n"
              << "  save                      - Save index to disk\n"
              << "  info                      - Display index statistics\n"
              << "  exit                      - Exit application\n";
}

void Application::handleInteractiveLoop() {
    std::cout << "Type 'help' for commands.\n";
    std::string input;
    while (true) {
        std::cout << "sfi> ";
        if (!std::getline(std::cin, input)) break;
        
        if (input.empty()) continue;
        if (input == "exit" || input == "quit") {
            m_serializer->saveToFile(m_indexFilePath);
            break;
        }
        
        processCommand(input);
    }
}

void Application::processCommand(const std::string& input) {
    std::istringstream iss(input);
    std::string cmd;
    iss >> cmd;

    if (cmd == "help") {
        printHelp();
    } else if (cmd == "index") {
        std::string path;
        std::getline(iss >> std::ws, path); 
        if (path.empty()) {
            std::cout << "Usage: index <path>\n";
        } else {
            size_t count = m_indexer->indexDirectory(path);
            std::cout << "Indexed " << count << " files.\n";
            m_serializer->saveToFile(m_indexFilePath);
        }
    } else if (cmd == "save") {
        if (m_serializer->saveToFile(m_indexFilePath)) {
            std::cout << "Index saved successfully.\n";
        }
    } else if (cmd == "info") {
        std::cout << "Index contains " << m_index->getFileCount() << " files.\n";
    } else if (cmd == "search") {
        std::string type;
        iss >> type;
        
        core::SearchQuery query;
        bool valid = true;
        
        if (type == "name") {
            std::string name;
            std::getline(iss >> std::ws, name);
            if (name.empty()) valid = false; else query.filename = name;
        } else if (type == "ext") {
            std::string ext;
            iss >> ext;
            if (ext.empty()) valid = false; else query.extension = ext;
        } else if (type == "path") {
            std::string path;
            std::getline(iss >> std::ws, path);
            if (path.empty()) valid = false; else query.pathContains = path;
        } else if (type == "size") {
            uintmax_t min, max;
            if (iss >> min >> max) {
                query.minSize = min;
                query.maxSize = max;
            } else {
                valid = false;
            }
        } else {
            valid = false;
            std::cout << "Unknown search type.\n";
        }
        
        if (!valid) {
            std::cout << "Invalid search command.\n";
            return;
        }
        
        auto results = m_searchEngine->search(query);
        std::cout << "Found " << results.size() << " results.\n";
        
        size_t displayCount = std::min<size_t>(results.size(), 10);
        for (size_t i = 0; i < displayCount; ++i) {
            std::cout << "  " << results[i].path << " (" << results[i].size << " bytes)\n";
        }
        if (results.size() > 10) {
            std::cout << "  ... and " << (results.size() - 10) << " more.\n";
        }
        
    } else {
        std::cout << "Unknown command. Type 'help'.\n";
    }
}

} // namespace sfi::app
