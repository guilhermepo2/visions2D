workspace "visions2D"
    architecture "x64"

    configurations
    {
        "Debug"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}.%{cfg.architecture}"

IncludeDirectories = {}
IncludeDirectories["SDL2"] = "%{prj.name}/thirdparty/SDL2-2.0.12/include"
IncludeDirectories["GLEW"] = "%{prj.name}/thirdparty/glew-2.1.0/include"
IncludeDirectories["spdlog"] = "%{prj.name}/thirdparty/spdlog/include"
LibDirectories = {}
LibDirectories["SDL2"] = "%{prj.name}/thirdparty/SDL2-2.0.12/lib/x64"
LibDirectories["GLEW"] = "%[prj.name]/thirdparty/glew-2.1.0/lib/Release/x64"

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
        "%{IncludeDirectories.spdlog}",
        "%{IncludeDirectories.SDL2}",
        "%{IncludeDirectories.GLEW}"
    }

    libdirs
    {
        "%{LibDirectories.SDL2}",
        "%{LibDirectories.GLEW}",
    }

    links
    {
        "SDL2.lib",
        "glew32.lib",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "visions2D_DEBUG"
        }