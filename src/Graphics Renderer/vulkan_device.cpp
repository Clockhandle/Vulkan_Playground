#include "Graphics Renderer/vulkan_device.h"

VulkanDevice::VulkanDevice(const VulkanPhysicalDevice& physicalDevice)
    :
    m_device(VK_NULL_HANDLE),
    m_graphicsQueue(VK_NULL_HANDLE)
{
    createLogicalDevice(physicalDevice);
}

VulkanDevice::~VulkanDevice()
{
    if(m_device != VK_NULL_HANDLE)
    {
        vkDestroyDevice(m_device, nullptr);
    }
}

VkDevice VulkanDevice::getHandle() const
{
    return m_device;
}

VkQueue VulkanDevice::getGraphicsQueue() const
{
    return m_graphicsQueue;
}

void VulkanDevice::createLogicalDevice(const VulkanPhysicalDevice &physicalDevice)
{
    const VulkanPhysicalDevice::QueueFamilyIndices indices = physicalDevice.getQueueFamilyIndices();

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
    if(vkCreateDevice(physicalDevice.getHandle(), &createInfo, nullptr, &m_device) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0 , &m_graphicsQueue);
}
