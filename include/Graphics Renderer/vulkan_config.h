#ifndef VULKAN_CONFIG_H
#define VULKAN_CONFIG_H
#include <vector>
#include <vulkan/vulkan.h>

inline const std::vector<const char*> g_requiredDeviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#endif // VULKAN_CONFIG_H