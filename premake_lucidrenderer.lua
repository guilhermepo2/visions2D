project "lucid-renderer"
        location "tools/lucid-renderer"
        kind "ConsoleApp"
        language "C++"
        warnings "Extra"

        targetdir("bin/" .. outputdir .. "/%{prj.name}")
        objdir("binobj/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.location}/**.h",
            "%{prj.location}/**.cpp",
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
            "%{IncludeDirectories.GLAD}",
            "%{IncludeDirectories.spdlog}",
            "%{IncludeDirectories.DEARIMGUI}",
            -- "%{IncludeDirectories.STBIMAGE}",
            "%{IncludeDirectories.GLM}",
            "%{IncludeDirectories.RAPIDJSON}",
            "%{IncludeDirectories.LUA}"
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