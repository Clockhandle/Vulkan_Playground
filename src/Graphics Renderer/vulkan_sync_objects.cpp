#include "Graphics Renderer/vulkan_sync_objects.h"
#include <stdexcept>
VulkanSyncObjects::VulkanSyncObjects(VkDevice device)
    :
    m_device(device)
{
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    createSyncObjects();
}

VulkanSyncObjects::~VulkanSyncObjects()
{
    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        if(m_imageAvailableSemaphores[i] != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(m_device, m_imageAvailableSemaphores[i], nullptr);
        }

        if(m_renderFinishedSemaphores[i] != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(m_device, m_renderFinishedSemaphores[i], nullptr);
        }

        if(m_inFlightFences[i] != VK_NULL_HANDLE)
        {
            vkDestroyFence(m_device, m_inFlightFences[i], nullptr);
        }
    }
}

VkSemaphore VulkanSyncObjects::getImageAvailableSemaphore(uint32_t frameIndex) const
{
    return m_imageAvailableSemaphores[frameIndex];
}
VkSemaphore VulkanSyncObjects::getRenderFinishedSemaphore(uint32_t frameIndex) const
{
    return m_renderFinishedSemaphores[frameIndex];
}
VkFence VulkanSyncObjects::getFence(uint32_t frameIndex) const
{
    return m_inFlightFences[frameIndex];
}
void VulkanSyncObjects::createSyncObjects()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; 

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image available semaphore for frame " + std::to_string(i) + "!");
        }
        if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render finished semaphore for frame " + std::to_string(i) + "!");
        }
        if (vkCreateFence(m_device, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create in-flight fence for frame " + std::to_string(i) + "!");
        }
    }
}
