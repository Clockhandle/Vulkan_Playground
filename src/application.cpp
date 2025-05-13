
#include "application.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

Application::Application() 
    :
    #ifdef DEBUG
        m_enableValidationLayers(true)
    #else
        m_enableValidationLayers(false)
    #endif
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
    }
}

void Application::cleanup()
{
    //Order matters!!!
    m_vulkanSwapChain.reset();
    m_vulkanDevice.reset();
    m_vulkanSurface.reset();
    m_vulkanDebugMessenger.reset();
    m_vulkanInstance.reset();
    m_window.reset();
}

