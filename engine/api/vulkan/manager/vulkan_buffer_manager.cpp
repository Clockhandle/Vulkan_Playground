#include "vulkan_buffer_manager.h"
#include <stdexcept>

VulkanBufferManager::VulkanBufferManager(VkDevice device)
    :
    m_device(device)
{
    if (m_device == VK_NULL_HANDLE) 
    {
        // Or handle this more gracefully depending on your error strategy
        throw std::runtime_error("VulkanBufferManager: VkDevice handle is null!");
    }
}

VulkanBufferManager::~VulkanBufferManager()
{
}

std::unique_ptr<VulkanBuffer> VulkanBufferManager::createVertexBuffer(const void *vertedData, VkDeviceSize dataSize)
{
    auto vertexBuffer = std::make_unique<VulkanBuffer>(m_device, dataSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    return vertexBuffer;
}
