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

include "v2d_standalone.lua"
include "premake_sandbox.lua"

-- include "premake_tools.lua"