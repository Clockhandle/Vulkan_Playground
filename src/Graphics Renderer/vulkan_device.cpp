#include "Graphics Renderer/vulkan_device.h"
#include "Graphics Renderer/vulkan_physical_device.h"
#include "Graphics Renderer/vulkan_config.h"
#include <set>
#include <vector>
VulkanDevice::VulkanDevice(const VulkanPhysicalDevice& physicalDevice)
    :
    m_device(VK_NULL_HANDLE),
    m_graphicsQueue(VK_NULL_HANDLE),
    m_presentQueue(VK_NULL_HANDLE)
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

VkQueue VulkanDevice::getPresentQueue() const
{
    return m_presentQueue;
}
void VulkanDevice::createLogicalDevice(const VulkanPhysicalDevice &physicalDevice)
{
    const VulkanPhysicalDevice::QueueFamilyIndices indices = physicalDevice.getQueueFamilyIndices();

    if(!indices.isComplete())
    {
        throw std::runtime_error("Physical device did not find all the require queue families.");
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for(uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo); 
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(g_requiredDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = g_requiredDeviceExtensions.data();

    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
    if(vkCreateDevice(physicalDevice.getHandle(), &createInfo, nullptr, &m_device) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0 , &m_graphicsQueue);
    vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0 , &m_presentQueue);
}
