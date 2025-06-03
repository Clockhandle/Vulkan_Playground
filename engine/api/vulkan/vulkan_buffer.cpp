#include "vulkan_buffer.h"
#include <stdexcept>

VulkanBuffer::VulkanBuffer(VkDevice device, VkDeviceSize deviceSize, VkBufferUsageFlags usage)
    :
    m_buffer(VK_NULL_HANDLE),
    m_device(device),
    m_size(deviceSize),
    m_usageFlags(usage)
{
    createBufferInfo();
}

VulkanBuffer::~VulkanBuffer()
{
    if(m_buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(m_device, m_buffer, nullptr);
    }
}

VulkanBuffer::VulkanBuffer(VulkanBuffer &&other) noexcept
    :
    m_buffer(other.m_buffer),
    m_device(other.m_device),
    m_size(other.m_size),
    m_usageFlags(other.m_usageFlags)
{
    other.m_buffer = VK_NULL_HANDLE;
}

VulkanBuffer &VulkanBuffer::operator=(VulkanBuffer &&other) noexcept
{
    if(this != &other)
    {
        if(m_buffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(m_device, m_buffer, nullptr);
        }

        m_buffer = other.m_buffer;
        m_device = other.m_device;
        m_size = other.m_size;
        m_usageFlags = other.m_usageFlags;

        other.m_buffer = VK_NULL_HANDLE;
    }
    return *this;
}

void VulkanBuffer::createBufferInfo()
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = m_size;
    bufferInfo.usage = m_usageFlags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if(vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffer handle!");
    }
}
