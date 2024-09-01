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
            "src"
        }
        
        links
        {
            "log" -- required for c++ android logging	
        }
        
        linkoptions
        {
            "--no-undefined" -- this flag is used just to cmake link libraries
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
