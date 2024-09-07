#include "Application.hpp"

#include "Core/Logging.hpp"

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
    }

    Application::~Application()
    {
        delete m_Window;

        s_Instance = nullptr;
    }

    void Application::Run()
    {
        while (!m_App->destroyRequested)
        {
            m_Window->PollEvents(m_App);

            m_Window->Render();
        }
    }

    void Application::OnEvent(AndroidEvent& e)
    {
        LIB_LOG_INFO("Event: %s", e.GetName());

        AndroidEventHandler handler(e);
    }

    Application& Application::Get()
    {
        return *s_Instance;
    }

}