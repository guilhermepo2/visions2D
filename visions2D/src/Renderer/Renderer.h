#pragma once
#pragma once
#include <SDL.h>
#undef main
#include <string>
#include <GL/glew.h>

namespace visions2D {
	
	class Shader;

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

		// etc... etc...

		SDL_Window* m_Window;
		SDL_GLContext m_GLContext;
	};
}