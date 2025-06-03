#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H

#include <vulkan/vulkan.h>

class VulkanBuffer
{
public:
    VulkanBuffer(VkDevice device, VkDeviceSize deviceSize, VkBufferUsageFlags usage);
    ~VulkanBuffer();

    VulkanBuffer(const VulkanBuffer&) = delete;
    VulkanBuffer& operator=(const VulkanBuffer&) = delete;
    VulkanBuffer(VulkanBuffer&& other) noexcept;
    VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;

    void createBufferInfo();
private:
    VkBuffer m_buffer;
    VkDevice m_device;
    VkDeviceSize m_size;
    VkBufferUsageFlags m_usageFlags;
};
#endif // VULKAN_BUFFER_H