#ifndef VULKAN_GRAPHICS_PIPELINE_H
#define VULKAN_GRAPHICS_PIPELINE_H

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "vulkan_shader_module.h"
// #include <string> // Keep if shader paths become configurable

class VulkanGraphicsPipeline 
{
public:
    VulkanGraphicsPipeline(VkDevice device, VkFormat swapChainImageFormat, VkExtent2D swapChainExtent);  
    ~VulkanGraphicsPipeline();

    VkPipeline getHandle() const { return m_graphicsPipeline; }
    VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; }
    VkRenderPass getRenderPassHandle() const { return m_renderPass; } 

private:
    void createRenderPass(); 
    void createGraphicsPipeline(); 

    // Helper functions
    std::vector<VkPipelineShaderStageCreateInfo> createShaderStages();
    VkPipelineVertexInputStateCreateInfo configureVertexInput();
    VkPipelineInputAssemblyStateCreateInfo configureInputAssembly();
    VkPipelineViewportStateCreateInfo configureViewportState();
    VkPipelineRasterizationStateCreateInfo configureRasterizationState();
    VkPipelineMultisampleStateCreateInfo configureMultisampleState();
    VkPipelineColorBlendStateCreateInfo configureColorBlendState(VkPipelineColorBlendAttachmentState& colorBlendAttachment);
    VkPipelineLayoutCreateInfo configurePipelineLayout();
    
private:
    VkDevice m_device;
    VkPipeline m_graphicsPipeline;
    VkPipelineLayout m_pipelineLayout;
    VkRenderPass m_renderPass;      
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;   

    std::unique_ptr<VulkanShaderModule> m_vertShaderModule;
    std::unique_ptr<VulkanShaderModule> m_fragShaderModule;
};

#endif // VULKAN_GRAPHICS_PIPELINE_H