# Smart File Indexer & Search

A high-performance, professional-grade file indexing and search engine built in modern C++20. 

This application was designed to demonstrate advanced software engineering practices, featuring a modular, object-oriented architecture, thread-safe memory management, and high-speed binary serialization. It allows users to index massive directories and instantly query them without rescanning the disk.

## Features

- **Blazing Fast Searches**: Search indexed files instantly by filename, extension, path, or size range.
- **In-Memory Caching**: Utilizes standard template library (STL) containers combined with `std::shared_mutex` for concurrent, thread-safe read operations.
- **Binary Serialization**: Bypasses slow text parsing by saving/loading the index directly to/from disk using a highly efficient custom binary format.
- **Modern C++20 Filesystem**: Uses `<filesystem>` for robust, cross-platform directory traversal and metadata extraction (size, extension, epoch modification times).
- **Interactive CLI (REPL)**: Provides a built-in Read-Eval-Print Loop for seamless user interaction and query processing.
- **SOLID Design Principles**: Highly decoupled architecture separating concerns into distinct layers (Core, IO, Utilities, App).

## Architecture

- **`FileIndexer`**: Recursively traverses directories, gracefully handling permission errors and building metadata.
- **`FileIndex`**: The core, thread-safe database maintaining the state of all indexed files in memory.
- **`SearchEngine`**: Evaluates custom `SearchQuery` objects against the database using optimized matching algorithms.
- **`Serializer`**: Handles the binary I/O operations, complete with magic-number validation to ensure data integrity across application restarts.

## Prerequisites

- A C++20 compatible compiler (e.g., GCC 14+, Clang 18+, MSVC 2022)
- CMake 3.20 or higher

## Build Instructions

This project uses CMake for cross-platform builds.

```bash
# Clone the repository
git clone https://github.com/yourusername/SmartFileIndexer.git
cd SmartFileIndexer

# Create a build directory
mkdir build
cd build

# Generate build files and compile the project
cmake ..
cmake --build . --config Release
```

## Usage

Run the generated executable to start the interactive prompt. 

### Available Commands:

| Command | Description | Example |
|---|---|---|
| `index <path>` | Recursively index a directory | `index C:\Projects` |
| `search name <name>` | Search by exact or partial filename | `search name main.cpp` |
| `search ext <ext>` | Search for a specific extension | `search ext .hpp` |
| `search path <path>` | Search for a string within the full path | `search path /core/` |
| `search size <min> <max>`| Search by file size range (in bytes) | `search size 1024 4096` |
| `save` | Save the current in-memory index to disk | `save` |
| `info` | Display statistics about the current index | `info` |
| `exit` | Save and exit the application | `exit` |

## Technical Highlights

- **RAII & Smart Pointers**: Ensures zero memory leaks and proper resource acquisition/release.
- **Thread Safety**: Uses `std::shared_lock` and `std::unique_lock` to protect shared state, enabling future expansion into multi-threaded indexing.
- **Exception Handling**: Employs robust try-catch blocks during filesystem operations to prevent unexpected crashes on locked or protected system files.
