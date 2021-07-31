#pragma once
#include <glad/glad.h>
#include <SDL.h>
#undef main
#include <SDL_ttf.h>
#include <string>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include "RenderData.h"
#include <vector>

namespace visions2D {
	
	class Shader;
	class VertexArray;
	class OrtographicCamera;

	class Renderer {
	public:
		Renderer();
		~Renderer();

		bool Initialize(float _ScreenWidth, float _ScreenHeight, const std::string& _WindowTitle);
		void PrepareToRender();
		void Render();
		void Swap();

		void Shutdown();

		// TODO temp?
		Shader* GetSpriteShader() { return m_SpriteShader; }
		OrtographicCamera* GetCamera() { return m_OrtographicCamera; }
		VertexArray* GetSpriteVertexArray() { return m_DefaultVertexArray; }

		std::vector<RenderData> SpriteRenderData;

	private:
		float m_ScreenWidth;
		float m_ScreenHeight;
		std::string m_WindowTitle;

		// textures...
		Shader* m_SpriteShader;
		VertexArray* m_DefaultVertexArray;
		OrtographicCamera* m_OrtographicCamera;

		// etc... etc...

		SDL_Window* m_Window;
		SDL_GLContext m_GLContext;

		Texture* WhiteTexture;
	};
}