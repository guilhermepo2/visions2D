-- Visions2D
-- Dependencies: SDL2, SDL2 TTF, GLEW (TODO: dynamically link? I don't know!)
-- Header Only dependencies: spdlog, dearimgui, stbimage, glm, rapidjsson

workspace "visions2D"
    architecture "x64"

    configurations
    {
        "Debug"
    }

    startproject "sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}.%{cfg.architecture}"

include "v2d_standalone.lua"
include "premake_sandbox.lua"

group "tools"
    include "premake_lucidrenderer.lua"
    include "resources_manager.lua"
    include "cli_projectcreator.lua"