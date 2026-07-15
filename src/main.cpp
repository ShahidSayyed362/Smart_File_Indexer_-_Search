#include "app/Application.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        sfi::app::Application app;
        app.run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
