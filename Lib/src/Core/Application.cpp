#include "Application.hpp"

static void AppCmdCallback(android_app* app, int32_t cmd)
{
    Lib::Application::Get().GetWindow()->EventCallback(app, cmd);
}

namespace Lib
{

    static Application* s_Instance = nullptr;

    Application::Application(android_app* app)
        : m_App(app)
    {
        s_Instance = this;

        m_Window = new Window(app);

        app->onAppCmd = &AppCmdCallback;

        int events;
        android_poll_source* source;

        do
        {
            if (ALooper_pollOnce(m_Window->Initialized() ? 1 : 0, nullptr, &events, (void**)&source) >= 0)
            {
                if (source != nullptr)
                    source->process(app, source);
            }
        } while (app->destroyRequested == 0);
    }

    Application::~Application()
    {
        delete m_Window;

        s_Instance = nullptr;
    }

    void Application::Run()
    {
        m_Window->PollEvents(m_App);

        m_Window->Render();
    }

    Application& Application::Get()
    {
        return *s_Instance;
    }
}