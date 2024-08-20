#include "main.h" // main.h includes jni and log to test cmake include dirs

extern "C" JNIEXPORT void JNICALL Java_Horizon_Main_NativeMain(void* args)
{
#ifdef DEBUG
    __android_log_write(ANDROID_LOG_INFO, "CPP", "oh hai!, I'm c++ (Debug Mode)");
#else
    __android_log_write(ANDROID_LOG_INFO, "CPP", "oh hai!, I'm c++ (Release Mode)");
#endif
}
