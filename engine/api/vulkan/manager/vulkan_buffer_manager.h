#ifndef VULKAN_BUFFER_MANAGER_H
#define VULKAN_BUFFER_MANAGER_H

#include <vulkan_buffer.h>
#include <memory>

class VulkanBufferManager {
public:
    VulkanBufferManager(VkDevice device);
    ~VulkanBufferManager();

    std::unique_ptr<VulkanBuffer> createVertexBuffer(const void* vertedData, VkDeviceSize dataSize);

private:
    VkDevice m_device;
};

#endif // VULKAN_BUFFER_MANAGER_H