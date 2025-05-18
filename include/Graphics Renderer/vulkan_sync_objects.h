#ifndef VULKAN_SYNC_OBJECTS_H
#define VULKAN_SYNC_OBJECTS_H

#include <vulkan/vulkan.h>

class VulkanSyncObjects {
public:
    VulkanSyncObjects(VkDevice device);
    ~VulkanSyncObjects();

    VkSemaphore getImageAvailableSemaphore() const;
    VkSemaphore getRenderFinishedSemaphore() const;
    VkFence getFence() const;
private:
    void createSyncObjects();
private:
    VkDevice m_device;
    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
    VkFence m_inFlightFence;
    // Add public methods and members here
};

#endif // VULKAN_SYNC_OBJECTS_H