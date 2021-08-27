// *******************************************************
// *******************************************************
// 
// Lucid 2D renderer Standalone
// 
// *******************************************************
// *******************************************************

#include "visions2D.h"
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

namespace LUCID_RENDERER {
	void Render(visions2D::Renderer* renderer) {
		visions2D::RenderData rd;
		
		rd.Texture = nullptr;
		rd.TextureScale = glm::vec2(100.0f, 100.0f);

		rd.WorldPosition = glm::vec2(-100.0f, -100.0f);
		rd.tint = visions2D::Color(1.0f, 0.0f, 0.0f, 1.0f);
		renderer->SpriteRenderData.push_back(rd);

		rd.WorldPosition = glm::vec2(100.0f, -100.0f);
		rd.tint = visions2D::Color(1.0f, 1.0f, 0.0f, 1.0f);
		renderer->SpriteRenderData.push_back(rd);

		rd.WorldPosition = glm::vec2(-100.0f, 100.0f);
		rd.tint = visions2D::Color(0.0f, 1.0f, 1.0f, 1.0f);
		renderer->SpriteRenderData.push_back(rd);

		rd.WorldPosition = glm::vec2(100.0f, 100.0f);
		rd.tint = visions2D::Color(1.0f, 0.0f, 1.0f, 1.0f);
		renderer->SpriteRenderData.push_back(rd);
	}
}

int main(void) {
	Log::Initialize();
	LOG_INFO("logger initialized!");

	visions2D::Renderer* lucidRenderer = new visions2D::Renderer();

	if (lucidRenderer->Initialize(1024, 576, "lucid2d renderer")) {
		bool b_IsRunning = true;
		visions2D::Texture* tilemapTexture = new visions2D::Texture();
		tilemapTexture->Load("./src/assets/Sprites/tilemap_packed.png");

		visions2D::Tilemap* theTilemap = new visions2D::Tilemap();
		theTilemap->LoadFromJSON("./src/assets/Map/testMap2.json");

		visions2D::Tilesheet* theTileSheet = new visions2D::Tilesheet(tilemapTexture);
		theTileSheet->LoadFromTiledJson("./src/assets/Map/tilemap_packed.json");

		visions2D::FramebufferSpecification spec;
		spec.Width = 800;
		spec.Height = 600;
		visions2D::Framebuffer* theFrameBuffer = nullptr;
		theFrameBuffer = new visions2D::Framebuffer(spec);

		visions2D::Color textureColor;

		while (b_IsRunning) {
			SDL_Event Event;
			while (SDL_PollEvent(&Event)) {

				visions2D::DearImGui::ProcessEvent(&Event);

				switch (Event.type) {
				case SDL_QUIT:
					b_IsRunning = false;
					break;
				case SDL_KEYDOWN:
					if (Event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
						b_IsRunning = false;
					}
					break;
				}

			}

			lucidRenderer->PrepareToRender();

			// theFrameBuffer->Bind();

			glClear(GL_COLOR_BUFFER_BIT);
			glEnable(GL_BLEND);
			glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

			LUCID_RENDERER::Render(lucidRenderer);
			
			lucidRenderer->Render();

			{
				ImGui::Begin("Info");
				ImGui::LabelText("Draw Calls:", std::to_string(lucidRenderer->GetDrawCalls()).c_str() );
				ImGui::End();
			}

			lucidRenderer->Swap();
		}

		lucidRenderer->Shutdown();
	}

	delete lucidRenderer;
	return 0;
}