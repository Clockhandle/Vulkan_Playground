#include "Graphics Renderer/vulkan_physical_device.h"
#include <vector>
#include <set>      
#include <iostream> 

const std::vector<const char*> g_requiredDeviceExtensions = {
};

VulkanPhysicalDevice::VulkanPhysicalDevice(VkInstance instance, const VkSurfaceKHR& surface)
    : 
    m_instance(instance),
    m_surface(surface),
    m_physicalDevice(VK_NULL_HANDLE)
{
    pickPhysicalDevice(m_surface); 

    if (m_physicalDevice != VK_NULL_HANDLE)
    {
        vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);
    }
    else
    {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
}

VkPhysicalDevice VulkanPhysicalDevice::getHandle() const
{
    return m_physicalDevice;
}

const VulkanPhysicalDevice::QueueFamilyIndices& VulkanPhysicalDevice::getQueueFamilyIndices() const
{
    return m_queueFamilyIndices;
}

const VkPhysicalDeviceProperties& VulkanPhysicalDevice::getDeviceProperties() const
{
    return m_deviceProperties;
}


void VulkanPhysicalDevice::pickPhysicalDevice(const VkSurfaceKHR& surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        // It's good practice to inform if no Vulkan-capable GPUs are found at all.
        // This could be an exception or a logged error depending on application design.
        // For now, the constructor throws if m_physicalDevice remains VK_NULL_HANDLE.
        return; 
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
    int bestScore = -1;

    // Optional: Print available devices during development for easier debugging
    // std::cout << "Available Physical Devices (" << deviceCount << "):" << std::endl;
    for (const auto& device : devices)
    {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(device, &props);
        // std::cout << "  - " << props.deviceName; // Optional debug print

        if (isDeviceSuitable(device, surface))
        {
            // std::cout << " (Suitable)" << std::endl; // Optional debug print
            int currentScore = 0;
            if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                currentScore = 1000; 
            } else if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
                currentScore = 500; 
            } else {
                currentScore = 100; 
            }
            if (currentScore > bestScore) {
                bestScore = currentScore;
                bestDevice = device;
            }
        }
        // else { // Optional debug print
        //     std::cout << " (Not suitable)" << std::endl;
        // }
    }

    if (bestDevice != VK_NULL_HANDLE) {
        m_physicalDevice = bestDevice;
        m_queueFamilyIndices = findQueueFamilies(m_physicalDevice, m_surface);
    }
}

bool VulkanPhysicalDevice::isDeviceSuitable(VkPhysicalDevice device, const VkSurfaceKHR& surface)
{
    QueueFamilyIndices indices = findQueueFamilies(device, surface);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    // Placeholder for checking required features if any:
    // VkPhysicalDeviceFeatures supportedFeatures;
    // vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
    // bool featuresSupported = supportedFeatures.geometryShader; // Example

    return indices.isComplete() && extensionsSupported /* && featuresSupported */;
}

VulkanPhysicalDevice::QueueFamilyIndices VulkanPhysicalDevice::findQueueFamilies(VkPhysicalDevice device, const VkSurfaceKHR& surface)
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }
        
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if(presentSupport)
        {
            indices.presentFamily = i;
        }
        
        if (indices.isComplete())
        {
            break;
        }
        i++;
    }
    return indices;
}

bool VulkanPhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    if (g_requiredDeviceExtensions.empty()) {
        return true;
    }
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(g_requiredDeviceExtensions.begin(), g_requiredDeviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
}