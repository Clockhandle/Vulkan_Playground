#ifndef VULKAN_COMMAND_BUFFERS_H
#define VULKAN_COMMAND_BUFFERS_H

#include <vulkan/vulkan.h>
#include "vulkan_physical_device.h"
#include "vulkan_framebuffers.h"
#include "vulkan_config.h"
#include "../../renderer/mesh.h"

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
    
    VkCommandBuffer getHandle(uint32_t frameIndex) const;
    const VkCommandBuffer* getHandlePointer(uint32_t frameIndex) const;
    void recordCommandBuffer(uint32_t frameIndex, uint32_t imageIndex, const Mesh* meshToDraw);
private:
    void createCommandPool(const VulkanPhysicalDevice& physicalDevice);
    void createCommandBuffers();
private:
    VkDevice m_device;
    VkRenderPass m_renderPass;
    const VulkanFramebuffer& m_vulkanFramebufferRef;
    VkExtent2D m_swapChainExtent;
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;
    VkPipeline m_graphicsPipeline;
};

#endif // VULKAN_COMMAND_BUFFERS_H