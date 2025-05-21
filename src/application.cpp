
#include "application.h"

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
    m_vulkanSwapChain = std::make_unique<VulkanSwapChain>(m_vulkanDevice->getHandle(), *m_vulkanPhysicalDevice, m_vulkanSurface->getHandle(), m_window->getWindow());
    m_vulkanGraphicsPipeline = std::make_unique<VulkanGraphicsPipeline>(m_vulkanDevice->getHandle(), m_vulkanSwapChain->getFormat(), m_vulkanSwapChain->getExtent());
    m_vulkanFramebuffer = std::make_unique<VulkanFramebuffer>(*m_vulkanSwapChain, m_vulkanDevice->getHandle(), m_vulkanGraphicsPipeline->getRenderPassHandle());
    m_vulkanCommandBuffers = std::make_unique<VulkanCommandBuffers>(m_vulkanDevice->getHandle(), *m_vulkanPhysicalDevice, m_vulkanGraphicsPipeline->getRenderPassHandle(), *m_vulkanFramebuffer, m_vulkanSwapChain->getExtent(), m_vulkanGraphicsPipeline->getHandle());
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
        std::cerr << "What the fuck? " <<  e.what() << '\n';
    }
    cleanup();
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

void Application::cleanup()
{
    //Order matters!!!
    m_vulkanSyncObjects.reset();
    m_vulkanCommandBuffers.reset();
    m_vulkanFramebuffer.reset();
    m_vulkanGraphicsPipeline.reset();
    m_vulkanSwapChain.reset();
    m_vulkanDevice.reset();
    m_vulkanSurface.reset();
    m_vulkanDebugMessenger.reset();
    m_vulkanInstance.reset();
    m_window.reset();
}

void Application::drawFrame()
{
    VkFence fence = m_vulkanSyncObjects->getFence(m_currentFrame);
    vkWaitForFences(m_vulkanDevice->getHandle(), 1, &fence, VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    vkAcquireNextImageKHR
    (m_vulkanDevice->getHandle(),
    m_vulkanSwapChain->getHandle(),
    UINT64_MAX, m_vulkanSyncObjects->getImageAvailableSemaphore(m_currentFrame),
    VK_NULL_HANDLE,
    &imageIndex);

    vkResetFences(m_vulkanDevice->getHandle(), 1, &fence);

    vkResetCommandBuffer(m_vulkanCommandBuffers->getHandle(m_currentFrame), 0);

    m_vulkanCommandBuffers->recordCommandBuffer(m_currentFrame, imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphore[] = {m_vulkanSyncObjects->getImageAvailableSemaphore(m_currentFrame)};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphore;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    //VkCommandBuffer commandBuffer = m_vulkanCommandBuffers->getHandle();
    submitInfo.pCommandBuffers = m_vulkanCommandBuffers->getHandlePointer(m_currentFrame);

    VkSemaphore signalSemaphore[] = {m_vulkanSyncObjects->getRenderFinishedSemaphore(m_currentFrame)};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphore;

    if(vkQueueSubmit(m_vulkanDevice->getGraphicsQueue(), 1, &submitInfo, fence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphore;

    VkSwapchainKHR swapChains[] = {m_vulkanSwapChain->getHandle()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;
    
    vkQueuePresentKHR(m_vulkanDevice->getPresentQueue(), &presentInfo);

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

}
