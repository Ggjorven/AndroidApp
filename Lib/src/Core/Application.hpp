#pragma once

#include "Core/Events.hpp"
#include "Core/Window.hpp"

namespace Lib
{

    class Application
    {
    public:
        explicit Application(struct android_app* app);
        ~Application();

        void Run();

        void OnEvent(AndroidEvent& e);

        inline Window* GetWindow() { return m_Window; }
        inline android_app* GetNativeApp() { return m_App; }

        static Application& Get();

    private:
        struct android_app* m_App = nullptr;
        Window* m_Window = nullptr;
    };

}