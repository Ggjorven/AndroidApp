require "vendor/premake-android-studio/android_studio"

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

workspace "AndroidApp"
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    filter "system:android"
        location ("build")
        
        gradleversion "com.android.tools.build:gradle:8.0.2"
        gradlewrapper { "distributionUrl=https://services.gradle.org/distributions/gradle-8.0.2-bin.zip"}
        
        assetpacks
        {
            ["pack"] = "install-time",
        }

        androidnamespace "premake.android.studio"

group ""
    include "Main"
group ""