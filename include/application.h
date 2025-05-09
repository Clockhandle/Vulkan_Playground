#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <iostream>
#include "window.h"
#include "Graphics Renderer/vulkan_instance.h"
#include "Graphics Renderer/vulkan_debug_messenger.h"
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
    void cleanup();
private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<VulkanInstance> m_vulkanInstance;
    std::unique_ptr<VulkanDebugMessenger> m_vulkanDebugMessenger;
    const bool m_enableValidationLayers;

};

#endif // APPLICATION_H