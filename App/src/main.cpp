#define LIB_EXPORT_SYMBOLS
#include "Export.hpp"

#include "Core/Application.hpp"

using namespace Lib;

// Android NativeActivity EntryPoint
void android_main(struct android_app* app)
{
    Application application = Application(app);
    application.Run();
}