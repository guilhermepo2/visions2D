// *******************************************************
// *******************************************************
// 
// Tool to create .json files with project resources
// 
// *******************************************************
// *******************************************************

#include <visions2D.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

struct ItemEntry {
	std::string ItemName;
	std::string ItemPath;
};

std::vector<ItemEntry> Fonts;
std::vector<ItemEntry> Textures;

int main(void) {
	Log::Initialize();
	LOG_INFO("logger initialized!");
	visions2D::Renderer* resourceManagerRenderer = new visions2D::Renderer();

	if(resourceManagerRenderer->Initialize(1024, 576, "resource manager")) {
		bool bIsRunning = true;

		while (bIsRunning) {
			SDL_Event Event;
			while (SDL_PollEvent(&Event)) {
				ImGui_ImplSDL2_ProcessEvent(&Event);
				switch (Event.type) {
				case SDL_QUIT:
					bIsRunning = false;
					break;
				}
			}

			resourceManagerRenderer->PrepareToRender();
			glClear(GL_COLOR_BUFFER_BIT);
			resourceManagerRenderer->Render();

			ImGui::DockSpaceOverViewport();
			static char buffer[256] = "item name";
			

			ImGui::ShowDemoWindow();

			{
				ImGui::Begin("Fonts");

				// input: file name - and search for a path
				ImGui::InputText("font name", buffer, IM_ARRAYSIZE(buffer));

				// button to add entry
				if (ImGui::Button("+")) {
					ItemEntry a;
					a.ItemName = buffer;
					a.ItemPath = "bb";
					Fonts.push_back(a);
				}

				// show a list of entries
				for (int i = 0; i < Fonts.size(); i++) {
					ImGui::Text("Entry %d", (i + 1));
					ImGui::Text("Name: %s", Fonts[i].ItemName);
					if (ImGui::Button("Remove")) {
						// TODO
					}
				}

				ImGui::End();
			}

			resourceManagerRenderer->Swap();
		}

		resourceManagerRenderer->Shutdown();
	}

	delete resourceManagerRenderer;
	return 0;
}