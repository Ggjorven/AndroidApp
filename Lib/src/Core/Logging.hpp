#pragma once

#include <android/log.h>

namespace Lib
{

    #if !defined(NDEBUG)
        #define LIB_LOG_TRACE(...)      __android_log_print(ANDROID_LOG_VERBOSE, "Lib", __VA_ARGS__)
        #define LIB_LOG_INFO(...)       __android_log_print(ANDROID_LOG_INFO, "Lib", __VA_ARGS__)
        #define LIB_LOG_WARN(...)       __android_log_print(ANDROID_LOG_WARN, "Lib", __VA_ARGS__)
        #define LIB_LOG_ERROR(...)      __android_log_print(ANDROID_LOG_ERROR, "Lib", __VA_ARGS__)
        #define LIB_LOG_FATAL(...)      __android_log_print(ANDROID_LOG_FATAL, "Lib", __VA_ARGS__)
    #else
        #define LIB_LOG_TRACE(...)      
        #define LIB_LOG_INFO(...)       
        #define LIB_LOG_WARN(...)       
        #define LIB_LOG_ERROR(...)      
        #define LIB_LOG_FATAL(...)      
    #endif

    #if !defined(NDEBUG)
        #define LIB_ASSERT(x)  if (!(x)) __android_log_assert(#x, "Lib", "Failed.")
    #else
        #define LIB_ASSERT(x)
    #endif

}