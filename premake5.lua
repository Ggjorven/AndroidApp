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
        
        gradleversion "com.android.tools.build:gradle:8.5.2"
        gradlewrapper { "distributionUrl=https://services.gradle.org/distributions/gradle-8.7-all.zip"}
        
        gradleproperties
        {
            "org.gradle.jvmargs=-Xmx4608m",
            "org.gradle.parallel=true"
        }

        assetpacks
        {
            ["pack"] = "install-time",
        }

        androidappid "Android.App.Horizon"
        androidnamespace "Android.App.Horizon"

group ""
    include "Main"
group ""