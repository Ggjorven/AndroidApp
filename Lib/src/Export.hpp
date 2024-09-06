#pragma once

// Note: If we don't export it here (in the main app) it doesn't work
// This symbol can't be found at runtime: long com.google.androidgamesdk.GameActivity.initializeNativeCode(java.lang.String, java.lang.String, java.lang.String, android.content.res.AssetManager, byte[], android.content.res.Configuration)
#if defined(LIB_EXPORT_SYMBOLS)
    #include <game-activity/GameActivity.cpp>
#endif