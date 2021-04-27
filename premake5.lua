workspace "visions2D"
    architecture "x64"

    configurations
    {
        "Debug"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}.%{cfg.architecture}"

IncludeDirectories = {}
IncludeDirectories["spdlog"] = "%{prj.name}/thirdparty/spdlog/include"
LibDirectories = {}

project "visions2D"
    location "visions2D"
    kind "ConsoleApp"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("binobj/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{IncludeDirectories.spdlog}"
    }

    libdirs
    {

    }

    links
    {

    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "visions2D_DEBUG"
        }