#include "Graphics Renderer/vulkan_swap_chain.h"
#include <cstdint>
#include <limits>
#include <algorithm>

VulkanSwapChain::VulkanSwapChain(VkDevice device, const VulkanPhysicalDevice& vulkanPhysicalDevice, VkSurfaceKHR surface, GLFWwindow* window)
    :
    m_swapChain(VK_NULL_HANDLE),
    m_device(device),
    m_surface(surface),
    m_window(window)
{
    createSwapChain(vulkanPhysicalDevice);
}

VulkanSwapChain::~VulkanSwapChain()
{
    for (auto imageView : m_swapChainImageViews)
    {
        vkDestroyImageView(m_device, imageView, nullptr);
    }
    
    if(m_swapChain != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
    }
}

void VulkanSwapChain::createSwapChain(const VulkanPhysicalDevice& vulkanPhysicalDevice)
{
    SwapchainSupportDetails swapChainSupport = VulkanPhysicalDevice::querySwapChainSupport(vulkanPhysicalDevice.getHandle(), m_surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    m_swapChainImageFormat = surfaceFormat.format;
    VkPresentModeKHR presentMode = chooseSwapSurfacePresentMode(swapChainSupport.presentModes);
    m_swapChainExtent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = m_swapChainExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    VulkanPhysicalDevice::QueueFamilyIndices indices = vulkanPhysicalDevice.getQueueFamilyIndices();
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if(vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    uint32_t actualImageCount = 0;
    vkGetSwapchainImagesKHR(m_device, m_swapChain, &actualImageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device, m_swapChain, &actualImageCount, m_swapChainImages.data());

    createImageViews();
}

void VulkanSwapChain::createImageViews()
{
    m_swapChainImageViews.resize(m_swapChainImages.size());
    for(size_t i = 0; i < m_swapChainImages.size(); ++i)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_swapChainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if(vkCreateImageView(m_device, &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image views!");
        }

    }
}

VkSwapchainKHR VulkanSwapChain::getHandle()
{
    return m_swapChain;
}

VkFormat VulkanSwapChain::getFormat()
{
    return m_swapChainImageFormat;
}

VkExtent2D VulkanSwapChain::getExtent()
{
    return m_swapChainExtent;
}

VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    for(const auto& availableFormat : availableFormats)
    {
        if(availableFormat.format == VK_FORMAT_B8G8R8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::chooseSwapSurfacePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    for(const auto& availablePresentMode : availablePresentModes)
    {
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);

        VkExtent2D actualExtent = 
        {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
