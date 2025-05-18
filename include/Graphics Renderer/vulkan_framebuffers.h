#ifndef VULKAN_FRAMEBUFFERS_H
#define VULKAN_FRAMEBUFFERS_H

#include<vulkan/vulkan.h>
#include<vector>
#include "vulkan_swap_chain.h"

class VulkanFramebuffer {
public:
    VulkanFramebuffer(const VulkanSwapChain &swapChain, VkDevice device, VkRenderPass renderPass);
    ~VulkanFramebuffer();

private:
    void createFramebuffers(const VulkanSwapChain& swapChain);

    VkDevice m_device;
    VkRenderPass m_renderPass;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;
};

#endif // VULKAN_FRAMEBUFFERS_H