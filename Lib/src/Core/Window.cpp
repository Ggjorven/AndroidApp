#include "Window.hpp"

#include "Core/Logging.hpp"

namespace Lib
{

    Window::Window(android_app* app)
    {
        // Init(app);
    }

    Window::~Window()
    {
        if (m_Initialized)
            Destroy();

        m_Initialized = false;
    }

    void Window::PollEvents(android_app* app) const
    {
        int events;
        android_poll_source* source;

        do {
            if (ALooper_pollAll(m_Initialized ? 1 : 0, nullptr, &events, (void**)&source) >= 0)
            {
                if (source != nullptr)
                    source->process(app, source);
            }
        } while (app->destroyRequested == 0);
    }

    void Window::Render()
    {
    }

    void Window::EventCallback(android_app* app, int32_t cmd)
    {
        switch (cmd) 
        {
        case APP_CMD_INIT_WINDOW:
            Init(app);      // The window is being shown, get it ready.
            break;
        case APP_CMD_TERM_WINDOW:
            Destroy();      // The window is being hidden or closed, clean it up.
            break;

        default:
            LIB_LOG_INFO("Event not handled: %d", cmd);
            break;
        }
    }

    void Window::Init(android_app* app)
    {
        m_Context.Init(app);

        m_Initialized = true;
    }

    void Window::Destroy()
    {
        m_Context.Destroy();

        m_Initialized = false;
    }

}