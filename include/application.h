#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <iostream>
#include "window.h"
#include "Graphics Renderer/vulkan_instance.h"
#include "Graphics Renderer/vulkan_debug_messenger.h"
#include "Graphics Renderer/vulkan_physical_device.h"
#include "Graphics Renderer/vulkan_device.h"
#include "Graphics Renderer/vulkan_surface.h"
#include "Graphics Renderer/vulkan_swap_chain.h"
#include "Graphics Renderer/vulkan_graphics_pipeline.h"
#include "Graphics Renderer/vulkan_framebuffers.h"
#include "Graphics Renderer/vulkan_command_buffers.h"
#include "Graphics Renderer/vulkan_sync_objects.h"
#include "Graphics Renderer/vulkan_config.h"

class Application
{
public:
    Application();
    ~Application();

    void run();
private:
    void initWindow();
    void initVulkan();
    void mainLoop();
    void recreateSwapChain();
    void cleanup();
    void cleanupSwapChain();
    void drawFrame();
private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<VulkanInstance> m_vulkanInstance;
    std::unique_ptr<VulkanDebugMessenger> m_vulkanDebugMessenger;
    std::unique_ptr<VulkanPhysicalDevice> m_vulkanPhysicalDevice;
    std::unique_ptr<VulkanDevice> m_vulkanDevice;
    std::unique_ptr<VulkanSurface> m_vulkanSurface;
    std::unique_ptr<VulkanSwapChain> m_vulkanSwapChain;
    std::unique_ptr<VulkanGraphicsPipeline> m_vulkanGraphicsPipeline;
    std::unique_ptr<VulkanFramebuffer> m_vulkanFramebuffer;
    std::unique_ptr<VulkanCommandBuffers> m_vulkanCommandBuffers;
    std::unique_ptr<VulkanSyncObjects> m_vulkanSyncObjects;
    const bool m_enableValidationLayers;
    uint32_t m_currentFrame;

};

#endif // APPLICATION_H