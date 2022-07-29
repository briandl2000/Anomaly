workspace "Anomaly"
    architecture "x64"
    startproject "Editor"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
    

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}

group "Dependencies"

group ""

project "Anomaly"
    location "Anomaly"
    kind "SharedLib"
    cppdialect "C++17"
    language "C++"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
    }

    links
    {
    }

    flags { "NoPCH" }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "ANO_PLATFORM_WINDOWS",
            "ANO_BUILD_DLL"
        }

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DIST"
        runtime "Release"
        optimize "on"

project "Game_Content"
    location "Game_Content"
    kind "SharedLib"
    cppdialect "C++17"
    language "C++"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
        "Anomaly/src"
    }

    links
    {
        "Anomaly"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "GAME_PLATFORM_WINDOWS",
            "GAME_BUILD_DLL"
        }

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DIST"
        runtime "Release"
        optimize "on"

project "Game_Executable"
    location "Game_Executable"
    kind "ConsoleApp"
    cppdialect "C++17"
    language "C++"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


    postbuildcommands 
    {
        '{COPY} "../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Anomaly/Anomaly.dll"  "../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"',
        '{COPY} "../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Game_Content/Game_Content.dll"  "../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"'
    }

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
        "Anomaly/src",
        "Game_Content/src"
    }

    links
    {
        "Anomaly",
        "Game_Content"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DIST"
        runtime "Release"
        optimize "on"

project "Editor"
    location "Editor"
    kind "ConsoleApp"
    cppdialect "C++17"
    language "C++"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    postbuildcommands 
    {
        '{COPY} "../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Anomaly/Anomaly.dll"  "../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"',
        '{COPY} "../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Game_Content/Game_Content.dll"  "../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"'
    }

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
        "Anomaly/src",
        "Game_Content/src"
    }

    links
    {
        "Anomaly",
        "Game_Content"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DIST"
        runtime "Release"
        optimize "on"