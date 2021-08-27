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
#include <filesystem>

struct ItemEntry {
	std::string ItemName;
	std::string ItemPath;
	int id;
};

std::vector<ItemEntry> Fonts;

static bool RemoveFontWithId(int id) {
	if (id < 0 || id >= Fonts.size()) {
		return false;
	}

	Fonts.erase(Fonts.begin() + id);

	return true;
}

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
			static char filepath[256] = "c:\\workspace\\";
			

			// ImGui::ShowDemoWindow();

			{
				ImGui::Begin("Fonts");

				// input: file name - and search for a path
				ImGui::InputText("font name", buffer, IM_ARRAYSIZE(buffer));
				ImGui::InputText("filepath", filepath, IM_ARRAYSIZE(filepath));

				// button to add entry
				if (ImGui::Button("+")) {
					ItemEntry a;
					a.ItemName = buffer;
					a.ItemPath = filepath;
					a.id = Fonts.size();
					Fonts.push_back(a);
				}

				for (int i = 0; i < Fonts.size(); i++) {
					ImGui::Text("Entry %d", (i + 1));
					ImGui::Text("Name: %s", Fonts[i].ItemName);
					ImGui::Text("Path: %s", Fonts[i].ItemPath);
					
					// this just works for the first one... why?
					if (ImGui::Button("Remove")) {
						LOG_INFO("Remove: {0}", i);
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