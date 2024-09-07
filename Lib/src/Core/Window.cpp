#include "Window.hpp"

#include "Core/Application.hpp"
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

        if (ALooper_pollOnce(0, nullptr, &events, (void**)&source) >= 0)
        {
            if (source != nullptr)
                source->process(app, source);

            // Handle input events
            ProcessInput(app);
        }
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

    void Window::ProcessInput(android_app* app) const
    {
        pthread_mutex_lock(&app->mutex);

        // Access the current input buffer
        android_input_buffer* inputBuffer = &app->inputBuffers[app->currentInputBuffer];

        // Check if there are motion events to process
        for (int i = 0; i < inputBuffer->motionEventsCount; ++i)
        {
            GameActivityMotionEvent* motionEvent = &inputBuffer->motionEvents[i];

            // Process motion events (e.g., touch events)
            int32_t action = motionEvent->action;
            int32_t pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            action &= AMOTION_EVENT_ACTION_MASK;

            // Handle touch events based on the action type
            switch (action)
            {
                case AMOTION_EVENT_ACTION_DOWN:
                case AMOTION_EVENT_ACTION_POINTER_DOWN: {
                    float x = motionEvent->pointers[pointerIndex].rawX;
                    float y = motionEvent->pointers[pointerIndex].rawY;
                    int32_t pointerId = motionEvent->pointers[pointerIndex].id;

                    // Create and dispatch a touch pressed event
                    AndroidTouchPressedEvent event(x, y, pointerId);
                    Application::Get().OnEvent(event);  // Dispatch the event
                    break;
                }
                case AMOTION_EVENT_ACTION_UP:
                case AMOTION_EVENT_ACTION_POINTER_UP: {
                    float x = motionEvent->pointers[pointerIndex].rawX;
                    float y = motionEvent->pointers[pointerIndex].rawY;
                    int32_t pointerId = motionEvent->pointers[pointerIndex].id;

                    // Create and dispatch a touch released event
                    AndroidTouchReleasedEvent event(x, y, pointerId);
                    Application::Get().OnEvent(event);  // Dispatch the event
                    break;
                }
                case AMOTION_EVENT_ACTION_MOVE: {
                    // Handle motion (dragging)
                    for (int j = 0; j < motionEvent->pointerCount; ++j) {
                        float x = motionEvent->pointers[j].rawX;
                        float y = motionEvent->pointers[j].rawY;
                        int32_t pointerId = motionEvent->pointers[j].id;

                        // Create and dispatch a touch moved event
                        AndroidTouchMovedEvent event(x, y, pointerId);
                        Application::Get().OnEvent(event);  // Dispatch the event
                    }
                    break;
                }
                case AMOTION_EVENT_ACTION_CANCEL: {
                    // Handle canceled touch events
                    for (int j = 0; j < motionEvent->pointerCount; ++j) {
                        float x = motionEvent->pointers[j].rawX;
                        float y = motionEvent->pointers[j].rawY;
                        int32_t pointerId = motionEvent->pointers[j].id;

                        // Create and dispatch a touch cancelled event
                        AndroidTouchCancelledEvent event(x, y, pointerId);
                        Application::Get().OnEvent(event);  // Dispatch the event
                    }
                    break;
                }
                default:
                    break;
            }
        }

        // Clear the motion events count and data after processing
        inputBuffer->motionEventsCount = 0;
        std::memset(inputBuffer->motionEvents, 0, sizeof(inputBuffer->motionEvents));

        // Check if there are key events to process
        for (int i = 0; i < inputBuffer->keyEventsCount; ++i) {
            GameActivityKeyEvent* keyEvent = &inputBuffer->keyEvents[i];

            // Handle key events
            if (keyEvent->action == AKEY_EVENT_ACTION_DOWN) {
                // Create and dispatch a key pressed event
                AndroidKeyPressedEvent event(keyEvent->keyCode, keyEvent->repeatCount);
                Application::Get().OnEvent(event);  // Dispatch the event
            } else if (keyEvent->action == AKEY_EVENT_ACTION_UP) {
                // Create and dispatch a key released event
                AndroidKeyReleasedEvent event(keyEvent->keyCode);
                Application::Get().OnEvent(event);  // Dispatch the event
            }
        }

        // Clear the key events count and data after processing
        inputBuffer->keyEventsCount = 0;
        std::memset(inputBuffer->keyEvents, 0, sizeof(inputBuffer->keyEvents));

        // Mark the input buffer as processed and switch to the next buffer
        app->currentInputBuffer = (app->currentInputBuffer + 1) % NATIVE_APP_GLUE_MAX_INPUT_BUFFERS;

        pthread_mutex_unlock(&app->mutex);
    }


}