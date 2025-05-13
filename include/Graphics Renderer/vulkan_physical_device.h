#ifndef VULKAN_PHYSICAL_DEVICE_H
#define VULKAN_PHYSICAL_DEVICE_H

#include <stdexcept>
#include <optional>
#include <cstdint>
#include <vector>
#include <string>   
#include <iostream> 
#include <vulkan/vulkan.h>
#include "vulkan_surface.h"

struct SwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;   
};

class VulkanPhysicalDevice
{
public:
    VulkanPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
    ~VulkanPhysicalDevice();

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    VkPhysicalDevice getHandle() const;
    const QueueFamilyIndices& getQueueFamilyIndices() const;
    const VkPhysicalDeviceProperties& getDeviceProperties() const;
    static SwapchainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    
private:
    void pickPhysicalDevice(VkSurfaceKHR surface); 
    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device); 

private:
    VkInstance m_instance;
    VkSurfaceKHR m_surface;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE; 
    QueueFamilyIndices m_queueFamilyIndices;
    VkPhysicalDeviceProperties m_deviceProperties; 
};

#endif // VULKAN_PHYSICAL_DEVICE_H