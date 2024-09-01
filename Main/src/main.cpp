#include "main.h" // main.h includes jni and log to test cmake include dirs

#include "../../Lib/src/library.h"

extern "C" JNIEXPORT void JNICALL Java_Android_App_Horizon_Main_NativeMain(void* args)
{
    int a = Lib::ExportFunc();

#ifdef DEBUG
    __android_log_write(ANDROID_LOG_INFO, "CPP", "oh hai!, I'm c++ (Debug Mode) %d");
    __android_log_write(ANDROID_LOG_INFO, "CPP", "oh hai!, I'm c++ (Debug Mode) %d");
    __android_log_write(ANDROID_LOG_INFO, "CPP", "oh hai!, I'm c++ (Debug Mode) %d");
    __android_log_write(ANDROID_LOG_INFO, "CPP", "oh hai!, I'm c++ (Debug Mode) %d");
    __android_log_write(ANDROID_LOG_INFO, "CPP", "oh hai!, I'm c++ (Debug Mode) %d");
#else
    __android_log_write(ANDROID_LOG_INFO, "CPP", "oh hai!, I'm c++ (Release Mode)");
#endif
}
