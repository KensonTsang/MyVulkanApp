#pragma once

#include "lve_device.hpp"
#include "lve_gameobject.hpp"
#include "lve_renderer.hpp"
#include "lve_swap_chain.hpp"
#include "lve_window.hpp"

// std
#include <memory>
#include <vector>

namespace lve {

class FirstApp {
 public:
  FirstApp();
  ~FirstApp();

  FirstApp(const FirstApp&) = delete;
  FirstApp& operator=(const FirstApp&) = delete;

  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();

 private:
  void loadGameObjects();

  LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
  LveDevice lveDevice{lveWindow};
  LveRenderer lveRenderer{lveWindow, lveDevice};

  std::vector<LveGameObject> gameObjects;
};

}  // namespace lve