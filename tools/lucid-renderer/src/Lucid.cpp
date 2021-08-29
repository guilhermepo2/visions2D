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
		
		renderer->DrawQuad(glm::vec2(-100.0f, -100.0f), glm::vec2(100.0f, 100.0f), visions2D::Color::RED);
		renderer->DrawQuad(glm::vec2(100.0f, -100.0f) , glm::vec2(100.0f, 100.0f), visions2D::Color::GREEN);
		renderer->DrawQuad(glm::vec2(-100.0f, 100.0f) , glm::vec2(100.0f, 100.0f), visions2D::Color::BLUE);
		renderer->DrawQuad(glm::vec2(100.0f, 100.0f)  , glm::vec2(100.0f, 100.0f), visions2D::Color(0.0f, 1.0f, 1.0f, 1.0f));

		renderer->DrawQuad(-200.0f, -200.0f);
	}
}

int main(void) {
	Log::Initialize();
	LOG_INFO("logger initialized!");

	visions2D::Renderer* lucidRenderer = new visions2D::Renderer();

	if (lucidRenderer->Initialize(1024, 576, "lucid2d renderer")) {
		bool b_IsRunning = true;

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