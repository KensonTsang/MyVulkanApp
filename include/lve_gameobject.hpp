#pragma once

#include <memory>

#include "lve_model.hpp"

namespace lve {

struct Transform2dComponent {
  glm::vec2 translation{};
  glm::vec2 scale{1.0f, 1.0f};
  float rotation;

  glm::mat2 mat2() {
    // scaleMat : [scaleX]  {  0  }
    //            [  0  ]   {scaleY}
    glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};  // {} by column not row

    const float c = glm::cos(rotation);
    const float s = glm::sin(rotation);
    glm::mat2 rotationMat{{c, s}, {-s, c}};

    return rotationMat * scaleMat;
  }
};

class LveGameObject {
 public:
  using id_t = unsigned int;

  static LveGameObject createGameObject() {
    static id_t currencyId = 0;
    return LveGameObject{currencyId++};
  }

  LveGameObject(const LveGameObject&) = delete;
  LveGameObject& operator=(const LveGameObject&) = delete;
  LveGameObject(LveGameObject&&) = default;
  LveGameObject& operator=(LveGameObject&&) = default;

  id_t getId() const { return id; }

  std::shared_ptr<LveModel> model{};
  glm::vec3 color{};
  Transform2dComponent transform2d{};

 private:
  LveGameObject(id_t objId) : id(objId) {}

  id_t id;
};

}  // namespace lve