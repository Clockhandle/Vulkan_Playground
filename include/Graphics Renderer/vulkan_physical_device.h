#ifndef VULKAN_PHYSICAL_DEVICE_H
#define VULKAN_PHYSICAL_DEVICE_H

#include <stdexcept>
#include <optional>
#include <cstdint>
#include <vector>
#include <string>   
#include <iostream> 
#include <vulkan/vulkan.h>

class VulkanPhysicalDevice
{
public:
    VulkanPhysicalDevice(VkInstance instance);
    ~VulkanPhysicalDevice();

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;

        bool isComplete() const
        {
            return graphicsFamily.has_value();
        }
    };

    VkPhysicalDevice getHandle() const;
    const QueueFamilyIndices& getQueueFamilyIndices() const;
    const VkPhysicalDeviceProperties& getDeviceProperties() const;

private:
    void pickPhysicalDevice(); 
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device); 

private:
    VkInstance m_instance;
    // VkSurfaceKHR m_surface; // Removed for now
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE; 
    QueueFamilyIndices m_queueFamilyIndices;
    VkPhysicalDeviceProperties m_deviceProperties; 
};

#endif // VULKAN_PHYSICAL_DEVICE_H