#include "Graphics Renderer/vulkan_sync_objects.h"
#include <stdexcept>
VulkanSyncObjects::VulkanSyncObjects(VkDevice device)
    :
    m_device(device),
    m_imageAvailableSemaphore(VK_NULL_HANDLE),
    m_renderFinishedSemaphore(VK_NULL_HANDLE),
    m_inFlightFence(VK_NULL_HANDLE)
{
    createSyncObjects();
}

VulkanSyncObjects::~VulkanSyncObjects()
{
    if(m_imageAvailableSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(m_device, m_imageAvailableSemaphore, nullptr);
    }

    if(m_renderFinishedSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(m_device, m_renderFinishedSemaphore, nullptr);
    }

    if(m_inFlightFence != VK_NULL_HANDLE)
    {
        vkDestroyFence(m_device, m_inFlightFence, nullptr);
    }
}

VkSemaphore VulkanSyncObjects::getImageAvailableSemaphore() const
{
    return m_imageAvailableSemaphore;
}
VkSemaphore VulkanSyncObjects::getRenderFinishedSemaphore() const
{
    return m_renderFinishedSemaphore;
}
VkFence VulkanSyncObjects::getFence() const
{
    return m_inFlightFence;
}
void VulkanSyncObjects::createSyncObjects()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; 

    if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
    vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS ||
    vkCreateFence(m_device, &fenceInfo, nullptr, &m_inFlightFence) != VK_SUCCESS) {
    throw std::runtime_error("failed to create semaphores!");
}
}
