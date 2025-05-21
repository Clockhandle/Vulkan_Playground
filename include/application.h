#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <iostream>
#include <vector> 
#include <vulkan/vulkan.h>

class Window;
class VulkanInstance;
class VulkanDebugMessenger;
class VulkanSurface;
class VulkanPhysicalDevice;
class VulkanDevice;
class VulkanSwapChain;
class VulkanGraphicsPipeline;
class VulkanFramebuffer;
class VulkanCommandBuffers;
class VulkanSyncObjects;
class VulkanShaderModule; 

#include "Graphics Renderer/vulkan_config.h" 
#include "Graphics Renderer/vulkan_shader_module.h"

class Application {
public:
    Application();
    ~Application();
    void run();

private:
    void initWindow();
    void initVulkan();
    void mainLoop();
    void drawFrame();
    void cleanup();
    void cleanupSwapChain();
    void recreateSwapChain();

    std::unique_ptr<Window> m_window;
    std::unique_ptr<VulkanInstance> m_vulkanInstance;
    std::unique_ptr<VulkanDebugMessenger> m_vulkanDebugMessenger;
    std::unique_ptr<VulkanSurface> m_vulkanSurface;
    std::unique_ptr<VulkanPhysicalDevice> m_vulkanPhysicalDevice;
    std::unique_ptr<VulkanDevice> m_vulkanDevice;
    std::unique_ptr<VulkanSwapChain> m_vulkanSwapChain;
    std::unique_ptr<VulkanGraphicsPipeline> m_vulkanGraphicsPipeline;
    std::unique_ptr<VulkanFramebuffer> m_vulkanFramebuffer;
    std::unique_ptr<VulkanCommandBuffers> m_vulkanCommandBuffers;
    std::unique_ptr<VulkanSyncObjects> m_vulkanSyncObjects;

    std::unique_ptr<VulkanShaderModule> m_globalVertShaderModule;
    std::unique_ptr<VulkanShaderModule> m_globalFragShaderModule;

    std::vector<std::unique_ptr<VulkanSwapChain>> m_retiredSwapChains;
    
    bool m_enableValidationLayers;
    size_t m_currentFrame = 0;
};

#endif // APPLICATION_H