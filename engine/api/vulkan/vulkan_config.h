#ifndef VULKAN_CONFIG_H
#define VULKAN_CONFIG_H
#include <vector>
#include <vulkan/vulkan.h>

inline const std::vector<const char*> g_requiredDeviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
#endif // VULKAN_CONFIG_H