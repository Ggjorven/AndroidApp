project "App"
	kind "ConsoleApp"
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

            "java/**.*",

            -- Has to be specified since the generated one is bad.
            "AndroidManifest.xml"
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
            "x86_64"
        }

        androiddependencies
        {
            "com.android.support:support-v4:28.0.0",
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
