project "Game"
	kind "ConsoleApp"
	language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    
    filter "system:android"
        androidsdkversion "34"
        androidminsdkversion "34"

        androidappid "Android.App.Game"
        androidnamespace "Android.App.Game"

        files 
        { 
            "src/**.h", 
            "src/**.hpp", 
            "src/**.cpp", 

            "java/**.java",

            -- Has to be specified since the generated one is bad.
            "AndroidManifest.xml"
        }

        includedirs
        {
            "src",

            "${ANDROID_NDK}/sources/vulkan/include",

            "$<TARGET_PROPERTY:game-activity::game-activity_static,INTERFACE_INCLUDE_DIRECTORIES>"
        }

        defines
        {
            "VK_USE_PLATFORM_ANDROID_KHR"
        }
        
        links
        {
            "android",
            "log",
            "vulkan"
        }

        linkoptions
        {
            "--no-undefined" -- this flag is used just to cmake link libraries
        }
        
        androiddependencies
        {
            "androidx.activity:activity:1.9.2",
            "androidx.core:core:1.13.1",
            "androidx.appcompat:appcompat:1.7.0",
            "com.google.android.material:material:1.12.0",
            "androidx.games:games-activity:3.0.5",
        }

        androidabis
        {
            'armeabi-v7a', 'arm64-v8a', 'x86', 'x86_64'
        }

        assetpackdependencies
        {
            "pack"
        }

        androidfindcmakepackages
        {
            "game-activity REQUIRED CONFIG"
        }

        androidlinkcmakepackages
        {
            "game-activity::game-activity_static"
        }

        androidmoduleversions
        { 
            "org.jetbrains.kotlin:kotlin-stdlib:1.8.22", 
            "org.jetbrains.kotlin:kotlin-stdlib-jdk7:1.8.22", 
            "org.jetbrains.kotlin:kotlin-stdlib-jdk8:1.8.22" 
        }

        androidprefab "true"

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

    filter "configurations:Dist"
        defines { "NDEBUG" }
		optimize "Full"
