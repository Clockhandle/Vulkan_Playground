#ifndef VULKAN_SWAP_CHAIN_H
#define VULKAN_SWAP_CHAIN_H

#include <vulkan/vulkan.h>
#include <vector>
#include "vulkan_physical_device.h"

class VulkanSwapChain
{
public:
    VulkanSwapChain(VkDevice device, const VulkanPhysicalDevice& vulkanPhysicalDevice, VkSurfaceKHR surface, GLFWwindow* window, VkSwapchainKHR oldSwapChain = VK_NULL_HANDLE);
    ~VulkanSwapChain();

    VkSwapchainKHR getHandle() const;
    VkFormat getFormat() const;
    VkExtent2D getExtent() const;
    const std::vector<VkImageView>& getImageViews() const;
private:
    void createSwapChain(const VulkanPhysicalDevice& vulkanPhysicalDevice, VkSwapchainKHR oldSwapChain);
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