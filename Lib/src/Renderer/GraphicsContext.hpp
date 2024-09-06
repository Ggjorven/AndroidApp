#pragma once

#include <vulkan_wrapper.h>

#include <game-activity/native_app_glue/android_native_app_glue.h>

namespace Lib
{

    class GraphicsContext
    {
    public:
        GraphicsContext();
        ~GraphicsContext();

        void Init(struct android_app* app);
        void Destroy();

    private:
        VkInstance m_Instance = VK_NULL_HANDLE;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_Device = VK_NULL_HANDLE;
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
    };

}