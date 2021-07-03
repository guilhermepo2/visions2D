project "visions2D"
    location "visions2D"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

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