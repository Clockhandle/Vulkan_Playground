#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H

#include <vulkan/vulkan.h>

class VulkanBuffer
{
public:
    VulkanBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize deviceSize, VkBufferUsageFlags usage);
    ~VulkanBuffer();

    VulkanBuffer(const VulkanBuffer&) = delete;
    VulkanBuffer& operator=(const VulkanBuffer&) = delete;
    VulkanBuffer(VulkanBuffer&& other) noexcept;
    VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;

    VkBuffer getHandle() const;
    VkDeviceMemory getMemoryHandle() const;
    VkDeviceSize getSize() const;

private:
    void createBufferInfo();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
private:
    VkBuffer m_buffer;
    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
    VkDeviceSize m_size;
    VkBufferUsageFlags m_usageFlags;

    VkDeviceMemory m_bufferMemory;
};
#endif // VULKAN_BUFFER_H