project "App"
	kind "ConsoleApp"
	language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    
    filter "system:android"
        androidsdkversion "29"
        androidminsdkversion "29"

        files 
        { 
            "src/**.h", 
            "src/**.hpp", 
            "src/**.cpp", 

            "java/**.*",
        }

        includedirs
        {
            "src"
        }
        
        links
        {
            "log" -- required for c++ logging	
        }
        
        buildoptions
        {
            "-std=c++20" -- flag mainly here to test cmake compile options
        }
        
        linkoptions
        {
            "--no-undefined" -- this flag is used just to cmake link libraries
        }
        
        androidabis
        {
            "arm64-v8a"
        }

        androiddependencies
        {
            "com.android.support:support-v4:27.1.0",
        }
        
        assetpackdependencies
        {
            "pack"
        }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

    filter "configurations:Dist"
        defines { "NDEBUG" }
		optimize "Full"
