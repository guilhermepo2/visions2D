-- Visions2D
-- Dependencies: SDL2, SDL2 TTF, GLEW (TODO: dynamically link? I don't know!)
-- Header Only dependencies: spdlog, dearimgui, stbimage, glm, rapidjsson

workspace "visions2D"
    architecture "x64"

    configurations
    {
        "Debug"
    }

    startproject "lucid-renderer"

outputdir = "%{cfg.buildcfg}-%{cfg.system}.%{cfg.architecture}"

IncludeDirectories = {}
IncludeDirectories["SDL2"] = "visions2D/thirdparty/SDL2-2.0.12/include"
IncludeDirectories["SDL2_TTF"] = "visions2D/thirdparty/SDL2_ttf-2.0.15/include"
IncludeDirectories["GLEW"] = "visions2D/thirdparty/glew-2.1.0/include"
IncludeDirectories["spdlog"] = "visions2D/thirdparty/spdlog/include"
IncludeDirectories["DEARIMGUI"] = "visions2D/thirdparty/dearimgui"
IncludeDirectories["STBIMAGE"] = "visions2D/thirdparty/stb"
IncludeDirectories["GLM"] = "visions2D/thirdparty/glm"
IncludeDirectories["RAPIDJSON"] = "visions2D/thirdparty/rapidjson-1.1.0/include"

LibDirectories = {}
LibDirectories["SDL2"] = "visions2D/thirdparty/SDL2-2.0.12/lib/x64"
LibDirectories["GLEW"] = "visions2D/thirdparty/glew-2.1.0/lib/Release/x64"
LibDirectories["SDL2_TTF"] = "visions2D/thirdparty/SDL2_ttf-2.0.15/lib/x64"

project "visions2D"
    location "visions2D"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("binobj/" .. outputdir .. "/%{prj.name}")
    warnings "Extra"

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/thirdparty/dearimgui/*.h",
        "%{prj.name}/thirdparty/dearimgui/*.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDirectories.spdlog}",
        "%{IncludeDirectories.SDL2}",
        "%{IncludeDirectories.SDL2_TTF}",
        "%{IncludeDirectories.GLEW}",
        "%{IncludeDirectories.DEARIMGUI}",
        "%{IncludeDirectories.STBIMAGE}",
        "%{IncludeDirectories.GLM}",
        "%{IncludeDirectories.RAPIDJSON}"
    }

    libdirs
    {
        "%{LibDirectories.SDL2}",
        "%{LibDirectories.SDL2_TTF}",
        "%{LibDirectories.GLEW}",
    }

    links
    {
        "SDL2.lib",
        "SDL2_ttf.lib",
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

project "sandbox"
        location "visions2D"
        kind "ConsoleApp"
        language "C++"
        warnings "Extra"

        targetdir("bin/" .. outputdir .. "/%{prj.name}")
        objdir("binobj/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.name}/**.h",
            "%{prj.name}/**.cpp",
        }

        links
        {
            "visions2D"
        }

        -- This is exactly the same includedirs as visions2D, have to fing a way to fix this!
        includedirs
        {
            "visions2D/src",
            "%{IncludeDirectories.SDL2}",
            "%{IncludeDirectories.SDL2_TTF}",
            "%{IncludeDirectories.GLEW}",
            "%{IncludeDirectories.spdlog}",
            "%{IncludeDirectories.DEARIMGUI}",
            "%{IncludeDirectories.STBIMAGE}",
            "%{IncludeDirectories.GLM}",
            "%{IncludeDirectories.RAPIDJSON}"
        }

        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"

            defines
            {
                "visions2D_DEBUG",
                "sandbox_DEBUG"
            }


project "lucid-renderer"
        location "visions2D"
        kind "ConsoleApp"
        language "C++"
        warnings "Extra"

        targetdir("bin/" .. outputdir .. "/%{prj.name}")
        objdir("binobj/" .. outputdir .. "/%{prj.name}")

        files
        {
            "tools/%{prj.name}/**.h",
            "tools/%{prj.name}/**.cpp",
        }

        links
        {
            "visions2D"
        }

        -- This is exactly the same includedirs as visions2D, have to fing a way to fix this!
        includedirs
        {
            "visions2D/src",
            "%{IncludeDirectories.SDL2}",
            "%{IncludeDirectories.SDL2_TTF}",
            "%{IncludeDirectories.GLEW}",
            "%{IncludeDirectories.spdlog}",
            "%{IncludeDirectories.DEARIMGUI}",
            "%{IncludeDirectories.STBIMAGE}",
            "%{IncludeDirectories.GLM}",
            "%{IncludeDirectories.RAPIDJSON}"
        }

        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"

            defines
            {
                "visions2D_DEBUG",
                "lucidrenderer_DEBUG"
            }