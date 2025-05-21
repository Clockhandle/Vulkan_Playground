#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <iostream>
#include <vector> // Was missing, but likely needed by other headers
#include <vulkan/vulkan.h> // For VkFormat etc.

// Forward declarations for Vulkan wrapper classes
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
class VulkanShaderModule; // Forward declare or include

#include "Graphics Renderer/vulkan_config.h" // For MAX_FRAMES_IN_FLIGHT, WINDOW_WIDTH, WINDOW_HEIGHT
#include "Graphics Renderer/vulkan_shader_module.h" // Include for std::unique_ptr<VulkanShaderModule>

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

    // Cached shader modules
    std::unique_ptr<VulkanShaderModule> m_globalVertShaderModule;
    std::unique_ptr<VulkanShaderModule> m_globalFragShaderModule;

    std::vector<std::unique_ptr<VulkanSwapChain>> m_retiredSwapChains;
    
    bool m_enableValidationLayers;
    size_t m_currentFrame = 0;
};

#endif // APPLICATION_H