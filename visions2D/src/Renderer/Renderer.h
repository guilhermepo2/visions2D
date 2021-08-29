#pragma once
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include <vector>

#include "RenderData.h"
#include "Window.h"
// todo: this shouldn't be here!
#include "SDL2Window.h"
#ifdef main
#undef main
#endif
#include <SDL_ttf.h>

namespace visions2D {
	
	class Shader;
	class VertexArray;
	class OrtographicCamera;

	class Renderer {
	public:
		Renderer();
		~Renderer();

		bool Initialize(float _ScreenWidth, float _ScreenHeight, const std::string& _WindowTitle);

		// Auxiliary functions to add quads to sprite render data?!
		void DrawQuad(glm::vec2 Position, glm::vec2 TexSize, Color Tint);
		void DrawQuad(float x, float y);
		void DrawQuad(glm::vec2 Position);

		void PrepareToRender();
		void Render();
		void Swap();

		void Shutdown();

		// TODO temp?
		Shader* GetSpriteShader() { return m_SpriteShader; }
		OrtographicCamera* GetCamera() { return m_OrtographicCamera; }
		VertexArray* GetSpriteVertexArray() { return m_DefaultVertexArray; }

		std::vector<RenderData> SpriteRenderData;
		int GetDrawCalls() const;

	private:
		Window* m_Window;

		// these are probably OpenGL specific
		Shader* m_SpriteShader;
		VertexArray* m_DefaultVertexArray;
		Texture* WhiteTexture;


		OrtographicCamera* m_OrtographicCamera;
	};
}