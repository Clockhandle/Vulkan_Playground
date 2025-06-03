#ifndef VULKAN_GRAPHICS_PIPELINE_H
#define VULKAN_GRAPHICS_PIPELINE_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <memory> // For std::unique_ptr if other members need it, not for shaders anymore

// Forward declare VulkanShaderModule if its full definition isn't needed here
// Or include its header if getHandle() is used, but we'll pass VkShaderModule directly.

class VulkanGraphicsPipeline {
public:
    // Constructor now takes VkShaderModule handles
    VulkanGraphicsPipeline(
        VkDevice device, 
        VkFormat swapChainImageFormat, 
        VkExtent2D swapChainExtent,
        VkShaderModule vertShaderModule, // Pass vertex shader module handle
        VkShaderModule fragShaderModule  // Pass fragment shader module handle
    );
    ~VulkanGraphicsPipeline();

    VkPipeline getHandle() const { return m_graphicsPipeline; }
    VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; }
    VkRenderPass getRenderPassHandle() const { return m_renderPass; }

private:
    void createRenderPass();
    void createGraphicsPipeline(); // This will use the passed-in shader modules

    // Helper methods for pipeline creation
    std::vector<VkPipelineShaderStageCreateInfo> createShaderStages();
    VkPipelineVertexInputStateCreateInfo configureVertexInput();
    VkPipelineInputAssemblyStateCreateInfo configureInputAssembly();
    VkPipelineViewportStateCreateInfo configureViewportState();
    VkPipelineRasterizationStateCreateInfo configureRasterizationState();
    VkPipelineMultisampleStateCreateInfo configureMultisampleState();
    VkPipelineColorBlendStateCreateInfo configureColorBlendState(VkPipelineColorBlendAttachmentState& colorBlendAttachment);
    VkPipelineLayoutCreateInfo configurePipelineLayout();

    VkDevice m_device;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;

    VkPipeline m_graphicsPipeline;
    VkPipelineLayout m_pipelineLayout;
    VkRenderPass m_renderPass;

    // Shader modules are now passed in, not owned by this class
    VkShaderModule m_vertModuleHandle; // Store the passed-in handle
    VkShaderModule m_fragModuleHandle; // Store the passed-in handle
};

#endif // VULKAN_GRAPHICS_PIPELINE_H