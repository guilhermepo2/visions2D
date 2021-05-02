#pragma once
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

namespace visions2D {
	static class DearImGui {
	public:
		static void Initialize(SDL_Window* window, SDL_GLContext GLContext);
		static void BeginRender(SDL_Window* window);
		static void Present();
	};
}