#ifndef VULKAN_BUFFER_MANAGER_H
#define VULKAN_BUFFER_MANAGER_H

#include <vulkan_buffer.h>
#include <memory>

class VulkanBufferManager {
public:
    VulkanBufferManager(VkDevice device, VkPhysicalDevice physicalDevice);
    ~VulkanBufferManager();

    std::unique_ptr<VulkanBuffer> createVertexBuffer(const void* vertexData, VkDeviceSize dataSize);

private:
    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
};

#endif // VULKAN_BUFFER_MANAGER_H