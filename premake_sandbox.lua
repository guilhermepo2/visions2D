project "sandbox"
        location "sandbox"
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
            "%{prj.name}/src",
            "visions2D/src",
            "%{IncludeDirectories.SDL2}",
            "%{IncludeDirectories.SDL2_TTF}",
            "%{IncludeDirectories.GLEW}",
            "%{IncludeDirectories.spdlog}",
            "%{IncludeDirectories.DEARIMGUI}",
            -- "%{IncludeDirectories.STBIMAGE}",
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
