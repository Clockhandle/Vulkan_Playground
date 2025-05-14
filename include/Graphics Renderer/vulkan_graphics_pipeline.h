#ifndef VULKAN_GRAPHICS_PIPELINE_H
#define VULKAN_GRAPHICS_PIPELINE_H

#include <vulkan/vulkan.h>

class VulkanGraphicsPipeline 
{
public:
    VulkanGraphicsPipeline();  
    ~VulkanGraphicsPipeline();
private:
    void createGraphicsPipeline();
private:
    VkDevice m_device;
};

#endif // VULKAN_GRAPHICS_PIPELINE_H