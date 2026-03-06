#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

#include <map>
#include <set>
#include <array>
#include <string>
#include <vector>
#include <chrono>
#include <format>
#include <optional>
#include <algorithm>
#include <iostream>

class Window {
    private:
        std::string name = "Title Text";
        uint32_t width = 800, height = 800;

        GLFWwindow* window;
        
        void cleanup();

    public:
        Window();
        Window(std::string name, int width, int height);

        void init();
        void run();
};