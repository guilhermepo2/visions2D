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
			lucidRenderer->GetSpriteShader()->SetColor("uColor", textureColor);

			// Rendering the tilemap
			tilemapTexture->SetActive();

			int currentData = 0;
			float StartingX = -(theTilemap->GetMapWidth() / 2) * theTileSheet->GetTileWidth();
			float StartingY = (theTilemap->GetMapHeight() / 2) * theTileSheet->GetTileHeight();
			glm::vec2 Position = glm::vec2(StartingX, StartingY);
			int data;

			// TODO: this should also be an application call
			// the application should choose what to render, not the renderer lol
			// have an static structure of render data?
			// what should be in this struct?
			// texture scale (vec2)
			// world scale (vec2)
			// world rotation (float)
			// world translation (vec2)
			// texture coordinates

			for (int i = 0; i < theTilemap->GetMapWidth(); i++) {
				for (int j = 0; j < theTilemap->GetMapHeight(); j++) {

					glm::mat4 atextureScale = glm::scale(glm::mat4(1.0f), glm::vec3(theTileSheet->GetTileWidth(), theTileSheet->GetTileHeight(), 1.0f));
					glm::mat4 aworldScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
					glm::mat4 aworldRotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
					glm::mat4 aworldTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, 0.0f));
					glm::mat4 aworld = (aworldTranslation * aworldRotation * aworldScale) * atextureScale;

					lucidRenderer->GetSpriteShader()->SetMatrix4("uWorldTransform", aworld);
					lucidRenderer->GetSpriteShader()->SetMatrix4("uCameraViewProjection", lucidRenderer->GetCamera()->GetCameraViewProjection());

					lucidRenderer->GetSpriteVertexArray()->SetActive();
					data = theTilemap->GetData(currentData);
					float* newTexCoord = theTileSheet->GetTexCoordsFromId(data);
					lucidRenderer->GetSpriteVertexArray()->SubTexCoords(newTexCoord);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

					Position.x += theTileSheet->GetTileWidth();
					currentData++;
				}
				Position.y -= theTileSheet->GetTileHeight();
				Position.x = StartingX;
			}

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

			visions2D::DearImGui::Present();
			lucidRenderer->Swap();
		}

		lucidRenderer->Shutdown();
	}

	delete lucidRenderer;
	return 0;
}