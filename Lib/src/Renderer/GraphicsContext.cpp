#include "GraphicsContext.hpp"

#include "Core/Logging.hpp"

#include <vector>

#define VK_CHECK_RESULT_HELPER_2(expr, exprStr, randomizedNr) \
    VkResult result##randomizedNr = expr; \
    if (result##randomizedNr != VK_SUCCESS) \
        LIB_LOG_ERROR("Expression %s failed with error code: %d", exprStr, result##randomizedNr)

#define VK_CHECK_RESULT_HELPER(expr, exprStr, randomizedNr) VK_CHECK_RESULT_HELPER_2(expr, exprStr, randomizedNr)

#if !defined(NDEBUG)
    #define VK_CHECK_RESULT(expr) VK_CHECK_RESULT_HELPER((expr), #expr, __COUNTER__)
#else
    #define VK_CHECK_RESULT(expr) expr
#endif

namespace Lib
{

    static std::vector<const char*> s_InstanceExtensions = { "VK_KHR_surface", "VK_KHR_android_surface" };
    static std::vector<const char*> s_DeviceExtensions = { "VK_KHR_swapchain" };

    GraphicsContext::GraphicsContext()
    {
    }

    GraphicsContext::~GraphicsContext()
    {
    }

    void GraphicsContext::Init(android_app* app)
    {
        // Load Android vulkan and retrieve vulkan API function pointers
        if (!InitVulkan()) 
        {
            LIB_LOG_ERROR("Vulkan is unavailable, install vulkan and re-start");
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext = nullptr;
        appInfo.pApplicationName = "AndroidVulkanApp";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 0);
        appInfo.pEngineName = "AndroidVulkanApp";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 1, 0);
        appInfo.apiVersion = VK_MAKE_VERSION(1, 1, 0);

        // Create the Vulkan instance
        VkInstanceCreateInfo instanceCreateInfo = {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledLayerCount = 0;
        instanceCreateInfo.ppEnabledLayerNames = nullptr;
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(s_InstanceExtensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = s_InstanceExtensions.data();

        VK_CHECK_RESULT(vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance));

        // if we create a surface, we need the surface extension
        VkAndroidSurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.window = app->window;

        VK_CHECK_RESULT(vkCreateAndroidSurfaceKHR(m_Instance, &createInfo, nullptr, &m_Surface));

        // Find one GPU to use:
        // On Android, every GPU device is equal -- supporting
        // graphics/compute/present
        // for this sample, we use the very first GPU device found on the system
        uint32_t gpuCount = 0;
        VK_CHECK_RESULT(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, nullptr));

        VkPhysicalDevice tmpGpus[gpuCount];
        VK_CHECK_RESULT(vkEnumeratePhysicalDevices(m_Instance, &gpuCount, tmpGpus));
        m_PhysicalDevice = tmpGpus[0];  // Pick up the first GPU Device, since android has only one (usually)?

        // check for vulkan info on this GPU device
        VkPhysicalDeviceProperties gpuProperties = {};
        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &gpuProperties);
        LIB_LOG_INFO("Vulkan Physical Device Name: %s", gpuProperties.deviceName);
        LIB_LOG_INFO("Vulkan Physical Device Info: apiVersion: %x \n\t driverVersion: %x", gpuProperties.apiVersion, gpuProperties.driverVersion);
        LIB_LOG_INFO("API Version Supported: %d.%d.%d", VK_VERSION_MAJOR(gpuProperties.apiVersion), VK_VERSION_MINOR(gpuProperties.apiVersion), VK_VERSION_PATCH(gpuProperties.apiVersion));

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, m_Surface, &surfaceCapabilities);

        LIB_LOG_INFO("Vulkan Surface Capabilities:\n");
        LIB_LOG_INFO("\timage count: %u - %u\n", surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);
        LIB_LOG_INFO("\tarray layers: %u\n", surfaceCapabilities.maxImageArrayLayers);
        LIB_LOG_INFO("\timage size (now): %dx%d\n", surfaceCapabilities.currentExtent.width, surfaceCapabilities.currentExtent.height);
        LIB_LOG_INFO("\timage size (extent): %dx%d - %dx%d\n", surfaceCapabilities.minImageExtent.width, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.width, surfaceCapabilities.maxImageExtent.height);
        LIB_LOG_INFO("\tusage: %x\n", surfaceCapabilities.supportedUsageFlags);
        LIB_LOG_INFO("\tcurrent transform: %u\n", surfaceCapabilities.currentTransform);
        LIB_LOG_INFO("\tallowed transforms: %x\n", surfaceCapabilities.supportedTransforms);
        LIB_LOG_INFO("\tcomposite alpha flags: %u\n", surfaceCapabilities.currentTransform);

        // Find a GFX queue family
        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

        LIB_ASSERT(queueFamilyCount);

        std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilyProperties.data());

        uint32_t queueFamilyIndex;
        for (queueFamilyIndex=0; queueFamilyIndex < queueFamilyCount; queueFamilyIndex++) 
        {
            if (queueFamilyProperties[queueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                break;
        }
        LIB_ASSERT((queueFamilyIndex < queueFamilyCount));

        // Create a logical device from GPU we picked
        float priorities[] = {
            1.0f,
        };
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.pNext = nullptr;
        queueCreateInfo.flags = 0;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = priorities;

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = nullptr;
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(s_DeviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = s_DeviceExtensions.data();
        deviceCreateInfo.pEnabledFeatures = nullptr;

        VK_CHECK_RESULT(vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_Device));
    }

    void GraphicsContext::Destroy()
    {
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        vkDestroyDevice(m_Device, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
    }

}