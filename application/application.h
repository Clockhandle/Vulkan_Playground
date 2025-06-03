#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <iostream>
#include <vector> 
#include <vulkan/vulkan.h>

#include "window.h" 
#include "vulkan_instance.h"
#include "vulkan_debug_messenger.h"
#include "vulkan_surface.h"
#include "vulkan_physical_device.h"
#include "vulkan_device.h"
#include "vulkan_swap_chain.h"
#include "vulkan_graphics_pipeline.h"
#include "vulkan_framebuffers.h"
#include "vulkan_command_buffers.h"
#include "vulkan_sync_objects.h"
#include "vulkan_config.h" 
#include "vulkan_shader_module.h"
#include "manager/vulkan_buffer_manager.h"
#include "renderer/mesh.h"

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
    std::unique_ptr<VulkanBufferManager> m_vulkanBufferManager;
    std::unique_ptr<VulkanSyncObjects> m_vulkanSyncObjects;

    std::unique_ptr<VulkanShaderModule> m_globalVertShaderModule;
    std::unique_ptr<VulkanShaderModule> m_globalFragShaderModule;
    
    std::unique_ptr<Mesh> m_triangleMesh;

    std::vector<std::unique_ptr<VulkanSwapChain>> m_retiredSwapChains;
    
    bool m_enableValidationLayers;
    size_t m_currentFrame = 0;

    const std::vector<Vertex> triangleVertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

};

#endif // APPLICATION_H