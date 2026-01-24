#include "lve_model.hpp"

#include <cassert>

namespace lve {

LveModel::LveModel(LveDevice& lveDevice, const std::vector<Vertex>& vertices)
    : lveDevice(lveDevice) {
  createVertexBuffers(vertices);
}
LveModel::~LveModel() {
  vkDestroyBuffer(lveDevice.device(), vertexBuffer, nullptr);
  vkFreeMemory(lveDevice.device(), vertexBufferMemory, nullptr);
}

void LveModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
  vertexCount = static_cast<uint32_t>(vertices.size());

  assert(vertexCount >= 3 && "Vertex count must at least 3");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

  // Ask GPU to create a vertex buffer
  lveDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         vertexBuffer, vertexBufferMemory);

  void* data;
  vkMapMemory(lveDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(lveDevice.device(), vertexBufferMemory);
}

void LveModel::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}
void LveModel::draw(VkCommandBuffer commandBuffer) {
  vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

// Descript the chunk of vertex memory, e.g number of memory chunk, size of memory, action sequence
// of reading the vertex
std::vector<VkVertexInputBindingDescription> LveModel::Vertex::getBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding = 0;  // Binding to 0 index of buffer
  bindingDescriptions[0].stride =
      sizeof(Vertex);  // specifies the number of bytes from one entry to the next
  bindingDescriptions[0].inputRate =
      VK_VERTEX_INPUT_RATE_VERTEX;  // Move the the next data entry after each vertex
  return bindingDescriptions;
}

// Descript how to extract a vertex data inside the memory, e.g first 64bit is vertex information,
// and second 64bit is normal, and third 64 bit is color
std::vector<VkVertexInputAttributeDescription> LveModel::Vertex::getAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
  attributeDescriptions[0].binding = 0;   // Tells Vulkan from which binding the per-vertex data
                                          // comes
  attributeDescriptions[0].location = 0;  // The location parameter references the location
                                          // directive of the input in the vertex shader
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, position);

  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);

  return attributeDescriptions;
}

}  // namespace lve