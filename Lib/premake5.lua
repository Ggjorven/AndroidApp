project "Lib"
	kind "StaticLib"
	language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    
    filter "system:android"
        androidsdkversion "34"
        androidminsdkversion "34"

        files 
        { 
            "src/**.h", 
            "src/**.hpp", 
            "src/**.cpp", 
        }

        includedirs
        {
            "src",
        }
        
        links
        {
            "log", -- required for c++ android logging	
        }

        defines
        {
            "VK_USE_PLATFORM_ANDROID_KHR"
        }
        
        linkoptions
        {
            "--no-undefined" -- this flag is used just to cmake link libraries
        }

        androidabis
        {
            'armeabi-v7a', 'arm64-v8a', 'x86', 'x86_64'
        }

        androiddependencies
        {
            -- "com.android.support:support-v4:28.0.0",
            "androidx.activity:activity:1.9.1",
            -- "androidx.games:games-activity:3.0.5",
            "androidx.appcompat:appcompat:1.7.0",
            "androidx.games:games-activity:3.0.5",
        }

        androidfindcmakepackages
        {
            "game-activity REQUIRED CONFIG"
        }

        androidlinkcmakepackages
        {
            "game-activity::game-activity_static"
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
