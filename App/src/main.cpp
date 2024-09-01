#include "main.h" // main.h includes jni and log to test cmake include dirs

#include "../../Lib/src/library.h"

extern "C" JNIEXPORT void JNICALL Java_Android_App_App_Main_NativeMain(void* args)
{
    int a = Lib::ExportFunc();
    int b = Lib::EXPORT2();

#ifdef DEBUG
    __android_log_write(ANDROID_LOG_INFO, "APP", "NativeMain (Debug)");
#else
    __android_log_write(ANDROID_LOG_INFO, "APP", "NativeMain (Release or Dist)");
#endif
}
