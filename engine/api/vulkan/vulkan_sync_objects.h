#ifndef VULKAN_SYNC_OBJECTS_H
#define VULKAN_SYNC_OBJECTS_H

#include <vulkan/vulkan.h>
#include <vector>
#include "vulkan_config.h"
class VulkanSyncObjects {
public:
    VulkanSyncObjects(VkDevice device);
    ~VulkanSyncObjects();

    VkSemaphore getImageAvailableSemaphore(uint32_t frameIndex) const;
    VkSemaphore getRenderFinishedSemaphore(uint32_t frameIndex) const;
    VkFence getFence(uint32_t frameIndex) const;
private:
    void createSyncObjects();
private:
    VkDevice m_device;
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
    // Add public methods and members here
};

#endif // VULKAN_SYNC_OBJECTS_H