// *******************************************************
// *******************************************************
// 
// Lucid 2D renderer Standalone
// 
// *******************************************************
// *******************************************************

#include "visions2D.h"

int main(void) {
	Log::Initialize();
	LOG_INFO("logger initialized!");

	visions2D::Renderer* sandbox = new visions2D::Renderer();
	visions2D::InputSystem* inputSystem = new visions2D::InputSystem();

	if (sandbox->Initialize(1024, 576, "sandbox")) {
		inputSystem->Initialize();

		bool b_IsRunning = true;

		while (b_IsRunning) {
			inputSystem->PrepareForUpdate();

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

			inputSystem->Update();

			if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_SPACE)) {
				LOG_INFO("spacebar was pressed and the input system works!");
			}

			sandbox->PrepareToRender();
			sandbox->Render();
			sandbox->Swap();
		}

		inputSystem->Shutdown();
		sandbox->Shutdown();
	}

	delete inputSystem;
	delete sandbox;
	return 0;
}