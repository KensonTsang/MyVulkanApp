#include "first_app.hpp"

#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>
#include <stdexcept>

namespace lve {

void sierpinski(std::vector<LveModel::Vertex>& vertices, int depth, glm::vec2 left, glm::vec2 right,
                glm::vec2 top) {
  if (depth <= 0) {
    vertices.push_back({{top}, {1.0f, 0.0f, 0.0f}});
    vertices.push_back({{right}, {0.0f, 1.0f, 0.0f}});
    vertices.push_back({{left}, {0.0f, 0.0f, 1.0f}});
  } else {
    auto left_top = 0.5f * (left + top);
    auto right_top = 0.5f * (right + top);
    auto left_right = 0.5f * (left + right);

    sierpinski(vertices, depth - 1, left_top, right_top, top);
    sierpinski(vertices, depth - 1, left_right, right, right_top);
    sierpinski(vertices, depth - 1, left, left_top, left_right);
  }
}

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

void FirstApp::run() {
  SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};

  while (!lveWindow.shouldClose()) {
    glfwPollEvents();
    if (auto commandBuffer = lveRenderer.beginFrame()) {
      // begin offscreen shadow pass
      // render shadow casting objects
      // end offscreen shadow pass

      lveRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }

  // CPU will block until all gpu operations has completed
  vkDeviceWaitIdle(lveDevice.device());
}

void FirstApp::loadGameObjects() {
  std::vector<LveModel::Vertex> vertices{};
  sierpinski(vertices, 0, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
  auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

  auto triangle = LveGameObject::createGameObject();
  triangle.model = lveModel;
  triangle.color = {.1f, .8f, .1f};
  triangle.transform2d.translation.x = .2f;
  triangle.transform2d.scale = {1.5f, 1.0f};
  triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
}

}  // namespace lve