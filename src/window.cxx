#define GLFW_INCLUDE_VULKAN
#define VMA_IMPLEMENTATION
#define GLM_FORCE_RADIANS

#include<GLFW/glfw3.h>

#include "window.h"

Window::Window() { }

Window::Window(std::string name, int width, int height) {
    this->name = name;
    this->width = width;
    this->height = height;
}

void Window::init() {
    if (glfwInit() == GLFW_FALSE)   throw std::runtime_error("Failed to initialize GLFW!");
    if (!glfwVulkanSupported())     throw std::runtime_error("Vulkan is not supported!");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    this->window = glfwCreateWindow(this->width, this->height, this->name.c_str(), nullptr, nullptr);
}

void Window::run() {
    while (!glfwWindowShouldClose(this->window)) {
        glfwPollEvents();
        
    }

    this->cleanup();
}

void Window::cleanup() {

}