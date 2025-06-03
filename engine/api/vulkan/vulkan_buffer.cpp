#include "vulkan_buffer.h"
#include <stdexcept>

VulkanBuffer::VulkanBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize deviceSize, VkBufferUsageFlags usage)
    :
    m_buffer(VK_NULL_HANDLE),
    m_device(device),
    m_physicalDevice(physicalDevice),
    m_size(deviceSize),
    m_usageFlags(usage),
    m_bufferMemory(VK_NULL_HANDLE)
{
    createBufferInfo();
}

VulkanBuffer::~VulkanBuffer()
{
    if(m_buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(m_device, m_buffer, nullptr);
    }
    if(m_bufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_device, m_bufferMemory, nullptr);
    }
}

VulkanBuffer::VulkanBuffer(VulkanBuffer &&other) noexcept
    :
    m_buffer(other.m_buffer),
    m_device(other.m_device),
    m_physicalDevice(other.m_physicalDevice),
    m_size(other.m_size),
    m_usageFlags(other.m_usageFlags),
    m_bufferMemory(other.m_bufferMemory)
{
    other.m_buffer = VK_NULL_HANDLE;
    other.m_bufferMemory = VK_NULL_HANDLE;
}

VulkanBuffer &VulkanBuffer::operator=(VulkanBuffer &&other) noexcept
{
    if(this != &other)
    {
        if(m_buffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(m_device, m_buffer, nullptr);
        }
        if(m_bufferMemory != VK_NULL_HANDLE)
        {
            vkFreeMemory(m_device, m_bufferMemory, nullptr);
        }

        m_buffer = other.m_buffer;
        m_device = other.m_device;
        m_physicalDevice = other.m_physicalDevice;
        m_size = other.m_size;
        m_usageFlags = other.m_usageFlags;
        m_bufferMemory = other.m_bufferMemory;

        other.m_buffer = VK_NULL_HANDLE;
        other.m_bufferMemory = VK_NULL_HANDLE;
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

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_device, m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if(vkAllocateMemory(m_device, &allocInfo, nullptr, &m_bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffery memory!");
    }

    vkBindBufferMemory(m_device, m_buffer, m_bufferMemory, 0);
}

uint32_t VulkanBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

    for(uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
    {
        if(typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}

VkBuffer VulkanBuffer::getHandle() const
{
    return m_buffer;
}

VkDeviceMemory VulkanBuffer::getMemoryHandle() const
{
    return m_bufferMemory;
}

VkDeviceSize VulkanBuffer::getSize() const
{
    return m_size;
}
