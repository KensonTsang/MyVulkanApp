#pragma once

#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "lve_device.hpp"
#include "lve_gameobject.hpp"
#include "lve_pipeline.hpp"
#include "lve_swap_chain.hpp"
#include "lve_window.hpp"

// std
#include <memory>
#include <vector>

namespace lve {

struct SimplePushConstantData {
  glm::mat2 transform{1.0f};
  glm::vec2 offset;
  alignas(16) glm::vec3 color;
};

class FirstApp {
 public:
  void run();

  FirstApp();
  ~FirstApp();

  FirstApp(const FirstApp&) = delete;
  FirstApp& operator=(const FirstApp&) = delete;

  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

 private:
  void loadGameObjects();
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void freeCommandBuffers();
  void drawFrame();
  void recreateSwapChain();
  void recordCommandBuffer(int imageIndex);
  void renderGameObjects(VkCommandBuffer commandBuffer);

  void sierpinski(std::vector<LveModel::Vertex>& vertices, int depth, glm::vec2 left,
                  glm::vec2 right, glm::vec2 top);

  LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
  LveDevice lveDevice{lveWindow};
  std::unique_ptr<LveSwapChain> lveSwapChain;
  std::unique_ptr<LvePipeline> lvePipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
  std::vector<LveGameObject> gameObjects;
};

}  // namespace lve