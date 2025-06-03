#include "vulkan_buffer_manager.h"
#include <stdexcept>
#include <cstring>

VulkanBufferManager::VulkanBufferManager(VkDevice device, VkPhysicalDevice physicalDevice)
    :
    m_device(device),
    m_physicalDevice(physicalDevice)
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

std::unique_ptr<VulkanBuffer> VulkanBufferManager::createVertexBuffer(const void *vertexData, VkDeviceSize dataSize)
{
    auto vertexBuffer = std::make_unique<VulkanBuffer>(m_device, m_physicalDevice, dataSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    void* mappedData = nullptr;

    VkResult mapResult = vkMapMemory(m_device, vertexBuffer->getMemoryHandle(), 0, dataSize, 0, &mappedData);

    memcpy(mappedData, vertexData, dataSize);

    vkUnmapMemory(m_device, vertexBuffer->getMemoryHandle());
    return vertexBuffer;
}
