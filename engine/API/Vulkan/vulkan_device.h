#ifndef VULKAN_DEVICE_H
#define VULKAN_DEVICE_H

#include <vulkan/vulkan.h>
#include "vulkan_physical_device.h"


class VulkanDevice
{
public:
    VulkanDevice(const VulkanPhysicalDevice& physicalDevice);
    ~VulkanDevice();

    VkDevice getHandle() const;
    VkQueue getGraphicsQueue() const; 
    VkQueue getPresentQueue() const;

private:
    void createLogicalDevice(const VulkanPhysicalDevice& physicalDevice);
private:
    VkDevice m_device;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
};

#endif //VULKAN_DEVICE_H