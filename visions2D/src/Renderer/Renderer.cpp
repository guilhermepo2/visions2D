#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Color.h"
#include "OrtographicCamera.h"
#include "Tilesheet.h"

#include <Log.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Utilities/Tilemap.h"


namespace visions2D {
	// temporaries
	Texture* text;
	Texture* tilemap;
	Tilemap* theTilemap;
	Tilesheet* sheet;
	Color textureColor;

	float DefaultTexCoords[] = {
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f
	};


	Renderer::Renderer() {}
	Renderer::~Renderer() {}

	bool Renderer::Initialize(float _ScreenWidth, float _ScreenHeight, const std::string& _WindowTitle) {
		m_ScreenWidth = _ScreenWidth;
		m_ScreenHeight = _ScreenHeight;
		m_WindowTitle = _WindowTitle;

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		SDL_WindowFlags WindowFlags = SDL_WINDOW_OPENGL;

		m_Window = SDL_CreateWindow(
			m_WindowTitle.c_str(),
			100, 100,
			static_cast<int>(m_ScreenWidth), static_cast<int>(m_ScreenHeight),
			WindowFlags
		);

		assert(m_Window, "[renderer] unable to create window: {0}", SDL_GetError());

		m_GLContext = SDL_GL_CreateContext(m_Window);
		SDL_GL_MakeCurrent(m_Window, m_GLContext);
		SDL_GL_SetSwapInterval(1);

		glewExperimental = GL_TRUE;
		assert(glewInit() == GLEW_OK, "[renderer] unable to initialize glew");
		glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);

		m_SpriteShader = new Shader();
		m_SpriteShader->Load("./src/DefaultAssets/Shaders/DefaultSprite.vert", "./src/DefaultAssets/Shaders/DefaultSprite.frag");

		#include "DefaultVertexArray.data"
		m_DefaultVertexArray = new VertexArray(vertices, 4, 4, texCoords, indices, 6);
		
		text = new Texture();
		text->Load("./src/DefaultAssets/chara_hero.png");
		tilemap = new Texture();
		tilemap->Load("./src/DefaultAssets/Sprites/tilemap_packed.png");

		sheet = new Tilesheet(tilemap);
		sheet->LoadFromTiledJson("./src/DefaultAssets/Map/tilemap_packed.json");

		theTilemap = new Tilemap();
		theTilemap->LoadFromJSON("./src/DefaultAssets/Map/testMap2.json");

		m_OrtographicCamera = new OrtographicCamera(m_ScreenWidth, m_ScreenHeight);
		m_OrtographicCamera->SetColor(0.0f, 0.0f, 0.0f, 1.0f);

		// IMGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		ImGui_ImplSDL2_InitForOpenGL(m_Window, m_GLContext);
		ImGui_ImplOpenGL3_Init("#version 330");
		LOG_INFO("[renderer] dearimgui initialized");

		LOG_INFO("[renderer] initialized!");
		return true;
	}

	void Renderer::Render() {
		m_SpriteShader->SetActive();
		m_SpriteShader->SetColor("uColor", textureColor);
		

		// dearimgui setting it up
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(m_Window);
		ImGui::NewFrame();

		glClearColor(
			m_OrtographicCamera->CameraBackgroundColor.rgba[0],
			m_OrtographicCamera->CameraBackgroundColor.rgba[1],
			m_OrtographicCamera->CameraBackgroundColor.rgba[2],
			m_OrtographicCamera->CameraBackgroundColor.rgba[3]
		);
		
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		// Drawing Tilemap
		tilemap->SetActive();

		int currentData = 0;
		float StartingX = -(theTilemap->GetMapWidth() / 2) * sheet->GetTileWidth(); 
		float StartingY = (theTilemap->GetMapHeight() / 2) * sheet->GetTileHeight();
		glm::vec2 Position = glm::vec2(StartingX, StartingY);
		int data;
		
		for (int i = 0; i < theTilemap->GetMapWidth(); i++) {
			for (int j = 0; j < theTilemap->GetMapHeight(); j++) {

				glm::mat4 atextureScale = glm::scale(glm::mat4(1.0f), glm::vec3(sheet->GetTileWidth(), sheet->GetTileHeight(), 1.0f));
				glm::mat4 aworldScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				glm::mat4 aworldRotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
				glm::mat4 aworldTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, 0.0f));
				glm::mat4 aworld = (aworldTranslation * aworldRotation * aworldScale) * atextureScale;
				m_SpriteShader->SetMatrix4("uWorldTransform", aworld);
				m_SpriteShader->SetMatrix4("uCameraViewProjection", m_OrtographicCamera->GetCameraViewProjection());

				m_DefaultVertexArray->SetActive();
				data = theTilemap->GetData(currentData);
				float* newTexCoord = sheet->GetTexCoordsFromId(data);
				m_DefaultVertexArray->SubTexCoords(newTexCoord);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				Position.x += sheet->GetTileWidth();
				currentData++;
			}
			Position.y -= sheet->GetTileHeight();
			Position.x = StartingX;
		}
		
		
		{
			ImGui::ColorEdit4("square color", textureColor.rgba);
			ImGui::ColorEdit4("clear color", m_OrtographicCamera->CameraBackgroundColor.rgba);
		}
		
		// rendering ImGui
		ImGui::Render();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			SDL_Window* BackupCurrentWindow = SDL_GL_GetCurrentWindow();
			SDL_GLContext BackupCurrentContext = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(BackupCurrentWindow, BackupCurrentContext);
		}
		// finished rendering imgui

		SDL_GL_SwapWindow(m_Window);
	}

	void Renderer::Shutdown() {
		LOG_INFO("[renderer] shutting down");
		SDL_GL_DeleteContext(m_GLContext);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}
}