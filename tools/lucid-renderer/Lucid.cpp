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

int main(void) {
	Log::Initialize();
	LOG_INFO("logger initialized!");

	visions2D::Renderer* lucidRenderer = new visions2D::Renderer();

	if (lucidRenderer->Initialize(1024, 576, "lucid2d renderer")) {
		bool b_IsRunning = true;
		visions2D::Texture* tilemapTexture = new visions2D::Texture();
		tilemapTexture->Load("./src/DefaultAssets/Sprites/tilemap_packed.png");

		visions2D::Tilemap* theTilemap = new visions2D::Tilemap();
		theTilemap->LoadFromJSON("./src/DefaultAssets/Map/testMap2.json");

		visions2D::Tilesheet* theTileSheet = new visions2D::Tilesheet(tilemapTexture);
		theTileSheet->LoadFromTiledJson("./src/DefaultAssets/Map/tilemap_packed.json");

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

			theFrameBuffer->Bind();

			glClear(GL_COLOR_BUFFER_BIT);
			glEnable(GL_BLEND);
			glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

			int currentData = 0;
			float StartingX = -(theTilemap->GetMapWidth() / 2) * theTileSheet->GetTileWidth();
			float StartingY = (theTilemap->GetMapHeight() / 2) * theTileSheet->GetTileHeight();
			glm::vec2 Position = glm::vec2(StartingX, StartingY);
			int data;

			for (int i = 0; i < theTilemap->GetMapWidth(); i++) {
				for (int j = 0; j < theTilemap->GetMapHeight(); j++) {
					visions2D::RenderData rd;
					rd.Texture = tilemapTexture;
					rd.TextureScale = glm::vec2(theTileSheet->GetTileWidth(), theTileSheet->GetTileHeight());
					data = theTilemap->GetData(currentData);
					rd.TexCoords = theTileSheet->GetTexCoordsFromId(data);

					rd.WorldRotation = 0.0f;
					rd.WorldPosition = glm::vec2(Position.x, Position.y);
					rd.WorldScale = glm::vec2(1.0f, 1.0f);

					rd.tint = textureColor;
					
					lucidRenderer->SpriteRenderData.push_back(rd);

					Position.x += theTileSheet->GetTileWidth();
					currentData++;
				}
				Position.y -= theTileSheet->GetTileHeight();
				Position.x = StartingX;
			}

			lucidRenderer->Render();
			theFrameBuffer->Unbind();

			ImGui::DockSpaceOverViewport();

			
			{
				ImGui::Begin("Scene");
				// showing the framebuffer with dear imgui
				unsigned int FramebufferTexture = theFrameBuffer->GetColorAttachmentID();
				ImGui::Image((void*)FramebufferTexture, ImVec2{ 800, 600 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::End();
			}
			

			{
				ImGui::Begin("Color Options");
				ImGui::ColorEdit4("square color", textureColor.rgba);
				ImGui::ColorEdit4("clear color", lucidRenderer->GetCamera()->CameraBackgroundColor.rgba);
				ImGui::End();
			}

			lucidRenderer->Swap();
		}

		lucidRenderer->Shutdown();
	}

	delete lucidRenderer;
	return 0;
}