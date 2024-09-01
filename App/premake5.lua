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

        androidappid "Android.App.App"
        androidnamespace "Android.App.App"

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
            "src"
        }
        
        links
        {
            "Lib",
        }

        linkoptions
        {
            "--no-undefined" -- this flag is used just to cmake link libraries
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
