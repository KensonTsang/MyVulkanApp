#include "lve_model.hpp"

#include <cassert>

namespace lve {

LveModel::LveModel(LveDevice& lveDevice, const LveModel::Builder& builder) : lveDevice(lveDevice) {
  createVertexBuffers(builder.vertices);
  createIndexBuffers(builder.indices);
}
LveModel::~LveModel() {
  vkDestroyBuffer(lveDevice.device(), vertexBuffer, nullptr);
  vkFreeMemory(lveDevice.device(), vertexBufferMemory, nullptr);
  if (hasIndexBuffer) {
    vkDestroyBuffer(lveDevice.device(), indexBuffer, nullptr);
    vkFreeMemory(lveDevice.device(), indexBufferMemory, nullptr);
  }
}

void LveModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
  vertexCount = static_cast<uint32_t>(vertices.size());

  assert(vertexCount >= 3 && "Vertex count must at least 3");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  lveDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         stagingBuffer, stagingBufferMemory);

  void* data;
  vkMapMemory(lveDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(lveDevice.device(), stagingBufferMemory);

  lveDevice.createBuffer(bufferSize,
                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

  lveDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
  vkDestroyBuffer(lveDevice.device(), stagingBuffer, nullptr);
  vkFreeMemory(lveDevice.device(), stagingBufferMemory, nullptr);
}

void LveModel::createIndexBuffers(const std::vector<uint32_t>& indices) {
  indexCount = static_cast<uint32_t>(indices.size());

  hasIndexBuffer = indexCount > 0;

  if (!hasIndexBuffer) {
    return;
  }

  VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  lveDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         stagingBuffer, stagingBufferMemory);

  void* data;
  vkMapMemory(lveDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(lveDevice.device(), stagingBufferMemory);

  lveDevice.createBuffer(bufferSize,
                         VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

  lveDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);
  vkDestroyBuffer(lveDevice.device(), stagingBuffer, nullptr);
  vkFreeMemory(lveDevice.device(), stagingBufferMemory, nullptr);
}

void LveModel::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

  if (hasIndexBuffer) {
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
  }
}
void LveModel::draw(VkCommandBuffer commandBuffer) {
  if (hasIndexBuffer) {
    vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
  } else {
    vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
  }
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