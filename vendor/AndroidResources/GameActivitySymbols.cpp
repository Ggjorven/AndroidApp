// GameActivity's C/C++ code
// Note: This file is used in Lib (and actually used in App, since if we include it here
// there is a symbol that can't be found: long com.google.androidgamesdk.GameActivity.initializeNativeCode(java.lang.String, java.lang.String, java.lang.String, android.content.res.AssetManager, byte[], android.content.res.Configuration)
// #include <game-activity/GameActivity.cpp>

#include <game-text-input/gametextinput.cpp>

// Glue from GameActivity to android_main()
// Passing GameActivity event from main thread to app native thread.
extern "C" 
{
    #include <game-activity/native_app_glue/android_native_app_glue.c>
}