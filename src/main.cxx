#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <iostream>

#include "window.h"

int main(int argc, char* argv[]) {
    Window window = Window();

    try {
        window.init();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    try {
        window.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}