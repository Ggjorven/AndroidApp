#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <functional>

#include <game-activity/GameActivity.h>

// TODO: Remove/replace
#define BIT(x) (1 << x)

#define DEFINE_BITWISE_OPS(Type) \
constexpr Type operator | (Type lhs, Type rhs) \
{ \
    return static_cast<Type>(static_cast<int>(lhs) | static_cast<int>(rhs)); \
} \
constexpr bool operator & (Type lhs, Type rhs) \
{ \
    return static_cast<int>(lhs) & static_cast<int>(rhs); \
} \
constexpr Type operator ^ (Type lhs, Type rhs) \
{ \
    return static_cast<Type>(static_cast<int>(lhs) ^ static_cast<int>(rhs)); \
}

namespace Lib
{

    enum class AndroidEventType : uint8_t {
        None = 0,
        WindowClose, WindowResize,
        KeyPressed, KeyReleased, KeyTyped,
        TouchPressed, TouchReleased, TouchMoved, TouchCancelled
    };

    enum class AndroidEventCategory : uint8_t
    {
        None = 0,
        Application = BIT(0),
        Input = BIT(1),
        Keyboard = BIT(2),
        Touch = BIT(3)
    };
    DEFINE_BITWISE_OPS(AndroidEventCategory);

    #define ANDROID_EVENT_CLASS_TYPE(type) \
        static AndroidEventType GetStaticType() { return AndroidEventType::type; } \
        virtual AndroidEventType GetEventType() const override { return GetStaticType(); } \
        virtual const char* GetName() const override { return #type; }

    #define ANDROID_EVENT_CLASS_CATEGORY(category) \
        virtual AndroidEventCategory GetCategoryFlags() const override { return category; }

    class AndroidEvent {
    public:
        AndroidEvent() = default;
        virtual ~AndroidEvent() = default;

        virtual AndroidEventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual AndroidEventCategory GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        bool IsInCategory(AndroidEventCategory category) {
            return GetCategoryFlags() & category;
        }

        bool Handled = false;
    };

    class AndroidEventHandler {
    public:
        AndroidEventHandler(AndroidEvent& e) : m_Event(e) {}

        template<typename TEvent, typename F>
        bool Handle(const F& func) {
            static_assert(std::is_base_of<AndroidEvent, TEvent>::value, "TEvent is not an event type.");
            if (m_Event.GetEventType() == TEvent::GetStaticType()) {
                m_Event.Handled = func(static_cast<TEvent&>(m_Event));
                return true;
            }
            return false;
        }

    private:
        AndroidEvent& m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const AndroidEvent& e) {
        return os << e.ToString();
    }

    // Window Resize Event
    class AndroidWindowResizeEvent : public AndroidEvent {
    public:
        AndroidWindowResizeEvent(uint32_t width, uint32_t height)
            : m_Width(width), m_Height(height) {}

        inline uint32_t GetWidth() const { return m_Width; }
        inline uint32_t GetHeight() const { return m_Height; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        ANDROID_EVENT_CLASS_TYPE(WindowResize)
        ANDROID_EVENT_CLASS_CATEGORY(AndroidEventCategory::Application)
    private:
        uint32_t m_Width, m_Height;
    };

    // Key Event Base Class
    class AndroidKeyEvent : public AndroidEvent {
    public:
        inline int32_t GetKeyCode() const { return m_KeyCode; }

        ANDROID_EVENT_CLASS_CATEGORY(AndroidEventCategory::Keyboard | AndroidEventCategory::Input)
    protected:
        AndroidKeyEvent(int32_t keycode) : m_KeyCode(keycode) {}

        int32_t m_KeyCode;
    };

    // Key Pressed Event
    class AndroidKeyPressedEvent : public AndroidKeyEvent {
    public:
        AndroidKeyPressedEvent(int32_t keycode, int repeatCount)
            : AndroidKeyEvent(keycode), m_RepeatCount(repeatCount) {}

        inline int GetRepeatCount() const { return m_RepeatCount; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        ANDROID_EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_RepeatCount;
    };

    // Key Released Event
    class AndroidKeyReleasedEvent : public AndroidKeyEvent {
    public:
        AndroidKeyReleasedEvent(int32_t keycode) : AndroidKeyEvent(keycode) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        ANDROID_EVENT_CLASS_TYPE(KeyReleased)
    };

    // Key Typed Event
    class AndroidKeyTypedEvent : public AndroidKeyEvent {
    public:
        AndroidKeyTypedEvent(int32_t keycode) : AndroidKeyEvent(keycode) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }

        ANDROID_EVENT_CLASS_TYPE(KeyTyped)
    };

    // Touch Event Base Class
    class AndroidTouchEvent : public AndroidEvent {
    public:
        inline float GetX() const { return m_X; }
        inline float GetY() const { return m_Y; }
        inline int32_t GetPointerId() const { return m_PointerId; }

        ANDROID_EVENT_CLASS_CATEGORY(AndroidEventCategory::Touch | AndroidEventCategory::Input)
    protected:
        AndroidTouchEvent(float x, float y, int32_t pointerId)
            : m_X(x), m_Y(y), m_PointerId(pointerId) {}

        float m_X, m_Y;
        int32_t m_PointerId;
    };

    // Touch Pressed Event
    class AndroidTouchPressedEvent : public AndroidTouchEvent {
    public:
        AndroidTouchPressedEvent(float x, float y, int32_t pointerId)
            : AndroidTouchEvent(x, y, pointerId) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "TouchPressedEvent: (" << m_X << ", " << m_Y << "), Pointer ID: " << m_PointerId;
            return ss.str();
        }

        ANDROID_EVENT_CLASS_TYPE(TouchPressed)
    };

    // Touch Released Event
    class AndroidTouchReleasedEvent : public AndroidTouchEvent {
    public:
        AndroidTouchReleasedEvent(float x, float y, int32_t pointerId)
            : AndroidTouchEvent(x, y, pointerId) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "TouchReleasedEvent: (" << m_X << ", " << m_Y << "), Pointer ID: " << m_PointerId;
            return ss.str();
        }

        ANDROID_EVENT_CLASS_TYPE(TouchReleased)
    };

    // Touch Moved Event
    class AndroidTouchMovedEvent : public AndroidTouchEvent {
    public:
        AndroidTouchMovedEvent(float x, float y, int32_t pointerId)
            : AndroidTouchEvent(x, y, pointerId) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "TouchMovedEvent: (" << m_X << ", " << m_Y << "), Pointer ID: " << m_PointerId;
            return ss.str();
        }

        ANDROID_EVENT_CLASS_TYPE(TouchMoved)
    };

    // Touch Cancelled Event
    class AndroidTouchCancelledEvent : public AndroidTouchEvent {
    public:
        AndroidTouchCancelledEvent(float x, float y, int32_t pointerId)
            : AndroidTouchEvent(x, y, pointerId) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "TouchCancelledEvent: (" << m_X << ", " << m_Y << "), Pointer ID: " << m_PointerId;
            return ss.str();
        }

        ANDROID_EVENT_CLASS_TYPE(TouchCancelled)
    };
}
