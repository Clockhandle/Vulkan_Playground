#include "vulkan_surface.h"

VulkanSurface::VulkanSurface(VkInstance instance, GLFWwindow *window)
    :
    m_instance(instance),
    m_surface(VK_NULL_HANDLE)
{
    if(glfwCreateWindowSurface(m_instance, window, nullptr, &m_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}
VulkanSurface::~VulkanSurface()
{
    if(m_surface != VK_NULL_HANDLE && m_instance != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    }
}

VkSurfaceKHR VulkanSurface::getHandle() const
{
    return m_surface;
}