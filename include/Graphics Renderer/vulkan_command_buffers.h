#ifndef VULKAN_COMMAND_BUFFERS_H
#define VULKAN_COMMAND_BUFFERS_H

#include <vulkan/vulkan.h>
#include "vulkan_physical_device.h"
#include "vulkan_framebuffers.h"
class VulkanCommandBuffers {
public:
    VulkanCommandBuffers(
        VkDevice device,
        const VulkanPhysicalDevice& physicalDevice, 
        VkRenderPass renderPass,
        const VulkanFramebuffer& swapChainFramebuffers,
        VkExtent2D swapChainExtent,
        VkPipeline graphicsPipeline);
    ~VulkanCommandBuffers();
    
    void recordCommandBuffer(uint32_t imageIndex);
private:
    void createCommandPool(const VulkanPhysicalDevice& physicalDevice);
    void createCommandBuffer();
private:
    VkDevice m_device;
    VkRenderPass m_renderPass;
    const VulkanFramebuffer& m_swapChainFramebuffers;
    VkExtent2D m_swapChainExtent;
    VkCommandPool m_commandPool;
    VkCommandBuffer m_commandBuffer;
    VkPipeline m_graphicsPipeline;
};

#endif // VULKAN_COMMAND_BUFFERS_H