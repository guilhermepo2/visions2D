#pragma once
#pragma once
#include <SDL.h>
#undef main
#include <string>
#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

namespace visions2D {
	
	class Shader;
	class VertexArray;

	class Renderer {
	public:
		Renderer();
		~Renderer();

		bool Initialize(float _ScreenWidth, float _ScreenHeight, const std::string& _WindowTitle);
		void Render();

		void Shutdown();

	private:
		float m_ScreenWidth;
		float m_ScreenHeight;
		std::string m_WindowTitle;

		// textures...
		Shader* m_SpriteShader;
		VertexArray* m_DefaultVertexArray;

		// etc... etc...

		SDL_Window* m_Window;
		SDL_GLContext m_GLContext;
	};
}