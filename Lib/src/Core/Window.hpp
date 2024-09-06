#pragma once

#include <cstdint>

#include "Renderer/GraphicsContext.hpp"

#include <game-activity/native_app_glue/android_native_app_glue.h>

namespace Lib
{

    class Window
    {
    public:
        Window(struct android_app* app);
        ~Window();

        void PollEvents(struct android_app* app) const;
        void Render();

        void EventCallback(struct android_app* app, int32_t cmd);

        inline bool Initialized() const { return m_Initialized; }

    private:
        void Init(struct android_app* app);
        void Destroy();

    private:
        bool m_Initialized = false;

        GraphicsContext m_Context = {};
    };

}