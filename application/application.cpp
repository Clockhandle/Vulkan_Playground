#include "application.h"


#include <stdexcept> 
#include <iostream>  

Application::Application() 
    :
    #ifdef DEBUG
        m_enableValidationLayers(true),
    #else
        m_enableValidationLayers(false),
    #endif
    m_currentFrame(0)
{
    std::cout << "Create Application constructor" << std::endl;
}

Application::~Application()
{
    cleanup();
}

void Application::initWindow()
{
    m_window = std::make_unique<Window>(WINDOW_WIDTH, WINDOW_HEIGHT, "TRALALERO TRALALA");
    if(!m_window || !m_window->getWindow())
    {
        throw std::runtime_error("failed to create window in application file.");
    }
}

void Application::initVulkan()
{
    m_vulkanInstance = std::make_unique<VulkanInstance>(m_enableValidationLayers);
    if(m_enableValidationLayers && m_vulkanInstance && m_vulkanInstance->getHandle() != VK_NULL_HANDLE)
    {
        try{
            m_vulkanDebugMessenger = std::make_unique<VulkanDebugMessenger>(m_vulkanInstance->getHandle());
        }
        catch (const std::exception& e)
        {
            std::cerr << "failed to create debug messenger in application file: " << e.what() << std::endl; 
        }
    }
    m_vulkanSurface = std::make_unique<VulkanSurface>(m_vulkanInstance->getHandle(), m_window->getWindow());
    m_vulkanPhysicalDevice = std::make_unique<VulkanPhysicalDevice>(m_vulkanInstance->getHandle(), m_vulkanSurface->getHandle());
    m_vulkanDevice = std::make_unique<VulkanDevice>(*m_vulkanPhysicalDevice);

    // Create global shader modules once
    try {
        m_globalVertShaderModule = std::make_unique<VulkanShaderModule>(m_vulkanDevice->getHandle(), "shaders/shader.vert.spv");
        m_globalFragShaderModule = std::make_unique<VulkanShaderModule>(m_vulkanDevice->getHandle(), "shaders/shader.frag.spv");
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to create global shader modules: " + std::string(e.what()));
    }
    
    recreateSwapChain(); 
    
    m_vulkanBufferManager = std::make_unique<VulkanBufferManager>(m_vulkanDevice->getHandle(), m_vulkanPhysicalDevice->getHandle());
    m_triangleMesh = std::make_unique<Mesh>(triangleVertices, *m_vulkanBufferManager);
    m_vulkanSyncObjects = std::make_unique<VulkanSyncObjects>(m_vulkanDevice->getHandle());
}

void Application::run()
{
    try
    {
        initWindow();
        initVulkan();
        mainLoop();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        // Consider re-throwing or exiting if initialization fails critically
    }
}

void Application::mainLoop()
{
    while(!m_window->shouldClose())
    {
        m_window->pollEvents();
        drawFrame();
    }
    if (m_vulkanDevice && m_vulkanDevice->getHandle() != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(m_vulkanDevice->getHandle());
    }
}

void Application::recreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window->getWindow(), &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(m_window->getWindow(), &width, &height);
        glfwWaitEvents();
    }

    VkSwapchainKHR oldSwapchainHandle = VK_NULL_HANDLE;
    std::unique_ptr<VulkanSwapChain> tempOldSwapChain = nullptr; // To hold the old swapchain temporarily

    if (m_vulkanSwapChain) {
        oldSwapchainHandle = m_vulkanSwapChain->getHandle();
        tempOldSwapChain = std::move(m_vulkanSwapChain); // Move ownership from m_vulkanSwapChain
    }

    m_vulkanSwapChain = std::make_unique<VulkanSwapChain>(
        m_vulkanDevice->getHandle(),
        *m_vulkanPhysicalDevice,
        m_vulkanSurface->getHandle(),
        m_window->getWindow(),
        oldSwapchainHandle 
    );

    if (m_vulkanDevice && m_vulkanDevice->getHandle() != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(m_vulkanDevice->getHandle());
    }

    if (tempOldSwapChain) {
        m_retiredSwapChains.push_back(std::move(tempOldSwapChain));
    }

    cleanupSwapChain(); 

    if (!m_globalVertShaderModule || !m_globalFragShaderModule) {
        throw std::runtime_error("Global shader modules not initialized before pipeline creation!");
    }

    m_vulkanGraphicsPipeline = std::make_unique<VulkanGraphicsPipeline>(
        m_vulkanDevice->getHandle(),
        m_vulkanSwapChain->getFormat(), 
        m_vulkanSwapChain->getExtent(), 
        m_globalVertShaderModule->getHandle(), 
        m_globalFragShaderModule->getHandle()  
    );
    m_vulkanFramebuffer = std::make_unique<VulkanFramebuffer>(
        *m_vulkanSwapChain, 
        m_vulkanDevice->getHandle(),
        m_vulkanGraphicsPipeline->getRenderPassHandle()
    );
    m_vulkanCommandBuffers = std::make_unique<VulkanCommandBuffers>(
        m_vulkanDevice->getHandle(), 
        *m_vulkanPhysicalDevice, 
        m_vulkanGraphicsPipeline->getRenderPassHandle(), 
        *m_vulkanFramebuffer, 
        m_vulkanSwapChain->getExtent(),
        m_vulkanGraphicsPipeline->getHandle()
    );
}

void Application::cleanup()
{
    if (m_vulkanDevice && m_vulkanDevice->getHandle() != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(m_vulkanDevice->getHandle());
    }

    m_vulkanSyncObjects.reset(); 
    cleanupSwapChain(); 
    m_vulkanSwapChain.reset();

    m_globalFragShaderModule.reset();
    m_globalVertShaderModule.reset();
    
    m_vulkanBufferManager.reset();
    m_triangleMesh.reset();
    m_retiredSwapChains.clear();
    m_vulkanDevice.reset();
    m_vulkanSurface.reset();
    m_vulkanDebugMessenger.reset();
    m_vulkanInstance.reset();
    m_window.reset();
}

void Application::cleanupSwapChain()
{
    m_vulkanCommandBuffers.reset(); 
    m_vulkanFramebuffer.reset();
    m_vulkanGraphicsPipeline.reset();
}

void Application::drawFrame()
{
    VkFence currentFrameFence = m_vulkanSyncObjects->getFence(m_currentFrame);
    vkWaitForFences(m_vulkanDevice->getHandle(), 1, &currentFrameFence, VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(
        m_vulkanDevice->getHandle(),
        m_vulkanSwapChain->getHandle(),
        UINT64_MAX, 
        m_vulkanSyncObjects->getImageAvailableSemaphore(m_currentFrame),
        VK_NULL_HANDLE, 
        &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return; 
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vkResetFences(m_vulkanDevice->getHandle(), 1, &currentFrameFence);

    vkResetCommandBuffer(m_vulkanCommandBuffers->getHandle(m_currentFrame), 0); 
    m_vulkanCommandBuffers->recordCommandBuffer(m_currentFrame, imageIndex, m_triangleMesh.get());

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_vulkanSyncObjects->getImageAvailableSemaphore(m_currentFrame)};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = m_vulkanCommandBuffers->getHandlePointer(m_currentFrame);

    VkSemaphore signalSemaphores[] = {m_vulkanSyncObjects->getRenderFinishedSemaphore(m_currentFrame)};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if(vkQueueSubmit(m_vulkanDevice->getGraphicsQueue(), 1, &submitInfo, currentFrameFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
    
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {m_vulkanSwapChain->getHandle()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;
    
    result = vkQueuePresentKHR(m_vulkanDevice->getPresentQueue(), &presentInfo);

    bool windowWasResized = m_window->wasResized();

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || windowWasResized) {
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}