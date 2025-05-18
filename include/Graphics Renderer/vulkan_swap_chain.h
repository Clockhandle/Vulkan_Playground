#ifndef VULKAN_SWAP_CHAIN_H
#define VULKAN_SWAP_CHAIN_H

#include <vulkan/vulkan.h>
#include <vector>
#include "vulkan_physical_device.h"

class VulkanSwapChain
{
public:
    VulkanSwapChain(VkDevice device, const VulkanPhysicalDevice& vulkanPhysicalDevice, VkSurfaceKHR surface, GLFWwindow* window);
    ~VulkanSwapChain();

    VkSwapchainKHR getHandle();
    VkFormat getFormat();
    VkExtent2D getExtent();
private:
    void createSwapChain(const VulkanPhysicalDevice& vulkanPhysicalDevice);
    void createImageViews();
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapSurfacePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
private:
    VkSwapchainKHR m_swapChain;
    VkDevice m_device;
    VkSurfaceKHR m_surface;
    GLFWwindow* m_window;

    std::vector<VkImage> m_swapChainImages;
    std::vector<VkImageView> m_swapChainImageViews;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
};


#endif // VULKAN_SWAP_CHAIN_H