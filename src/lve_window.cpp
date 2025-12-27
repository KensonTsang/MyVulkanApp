#include "lve_window.hpp"

#include <iostream>
#include <stdexcept>

namespace lve {

LveWindow::LveWindow(int w, int h, std::string name) : width(w), height(h), windowName(name) {
  initWindow();
}

LveWindow::~LveWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void LveWindow::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(
      window,
      this);  // Since GLFWframebuffersizefun callback is a static function, I
              // need to set the pointer to glfw library, then framebufferResizeCallback
              // can pass the window pointer to the callback function
  glfwSetFramebufferSizeCallback(
      window,
      framebufferResizeCallback);  // everytime glfw get resize will call this resizeCallback
}

void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface");
  }
}

void LveWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
  auto lveWindow = reinterpret_cast<LveWindow*>(glfwGetWindowUserPointer(window));
  lveWindow->resetWindowResizedFlag();
  lveWindow->width = width;
  lveWindow->height = height;

  std::cout << "frameBuffer resized! new width: " << width << " new height: " << height << '\n';
}

}  // namespace lve