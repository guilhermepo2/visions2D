#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Color.h"
#include "OrtographicCamera.h"

#include <Log.h>
#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"


namespace visions2D {
	// temporaries
	Texture* awesomeFace;
	Texture* text;
	Texture* tilemap;
	Color textureColor;

	// Sprite information...
	glm::vec2 Position = glm::vec2(0.0f, 0.0f);
	glm::vec2 SpriteScale = glm::vec2(1.0f, 1.0f);
	float SpriteRotation = 0.0f;

	float DefaultTexCoords[] = {
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f
	};

	int mapData[] = { 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 14, 15, 15, 15, 2, 3, 4, 52, 35, 35, 126, 35, 52, 52, 52, 35, 35, 18, 15, 14, 14, 15, 15, 15, 19, 20, 21, 35, 35, 49, 92, 49, 52, 52, 35, 52, 52, 52, 15, 14, 14, 15, 15, 15, 19, 20, 21, 52, 52, 52, 35, 52, 18, 66, 35, 66, 52, 52, 15, 14, 14, 15, 15, 15, 19, 20, 21, 35, 35, 52, 52, 52, 52, 92, 52, 92, 52, 35, 15, 14, 14, 15, 15, 15, 36, 55, 38, 52, 52, 52, 18, 18, 35, 52, 52, 52, 52, 52, 15, 14, 14, 15, 15, 15, 52, 105, 68, 35, 52, 52, 52, 52, 52, 35, 35, 35, 52, 52, 15, 14, 14, 15, 15, 15, 35, 105, 35, 52, 18, 52, 52, 35, 35, 52, 1, 81, 52, 52, 15, 14, 14, 15, 15, 15, 52, 105, 52, 52, 52, 11, 13, 35, 35, 35, 97, 98, 99, 52, 15, 14, 14, 15, 15, 15, 52, 105, 52, 52, 52, 45, 47, 52, 52, 31, 114, 115, 116, 52, 15, 14, 14, 15, 15, 15, 52, 105, 52, 35, 35, 35, 35, 35, 35, 136, 100, 117, 101, 52, 15, 14, 14, 15, 15, 15, 52, 105, 52, 52, 52, 52, 52, 52, 52, 48, 112, 110, 112, 52, 15, 14, 14, 15, 15, 15, 52, 105, 52, 52, 52, 52, 52, 52, 52, 52, 31, 90, 31, 52, 15, 14, 14, 15, 15, 15, 35, 103, 88, 88, 88, 88, 88, 88, 88, 88, 88, 106, 31, 52, 15, 14, 14, 15, 15, 15, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 15, 14, 14, 15, 15, 15, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 35, 35, 15, 14, 14, 15, 15, 15, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 35, 52, 52, 15, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 };
	int mapWidth = 20;
	int mapHeight = 20;
	int mapTileWidth = 16;
	int mapTileHeight = 16;
	int tilesetColumns = 17;


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
		awesomeFace = new Texture();
		awesomeFace->Load("./src/DefaultAssets/awesomeface.png");
		tilemap = new Texture();
		tilemap->Load("./src/DefaultAssets/Sprites/tilemap_packed.png");

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
		float StartingX = - 10.0f * 16.0f;
		float StartingY = 10.0f * 16.0f;
		glm::vec2 Position = glm::vec2(StartingX, StartingY);
		glm::vec2 TileScale = glm::vec2(16.0f, 16.0f);

		for (int i = 0; i < mapWidth; i++) {
			for (int j = 0; j < mapHeight; j++) {
				// LOG_INFO("Current Position: ({0}, {1})", Position.x, Position.y);

				glm::mat4 atextureScale = glm::scale(glm::mat4(1.0f), glm::vec3(TileScale.x, TileScale.y, 1.0f));
				glm::mat4 aworldScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				glm::mat4 aworldRotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
				glm::mat4 aworldTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, 0.0f));
				glm::mat4 aworld = (aworldTranslation * aworldRotation * aworldScale) * atextureScale;
				m_SpriteShader->SetMatrix4("uWorldTransform", aworld);
				m_SpriteShader->SetMatrix4("uCameraViewProjection", m_OrtographicCamera->GetCameraViewProjection());

				m_DefaultVertexArray->SetActive();

				// TODO: Calculate Tex Coords!
				float tw = TileScale.x / tilemap->GetWidth(); // 0.0588
				float th = TileScale.y / tilemap->GetHeight(); // .125
				// int xPosition = mapData[currentData] % 8;
				int xPosition = (mapData[currentData] % 17) - 1; // has to be between 0 and 16
				int yPosition = 7 - (mapData[currentData] / 17);

				float NewTexCoords[] = {
					(xPosition + 1) * tw, (yPosition + 1) * th,
					(xPosition + 1) * tw, yPosition * th,
					xPosition * tw , yPosition * th,
					xPosition * tw, (yPosition + 1) * th
				};

				m_DefaultVertexArray->SubTexCoords(NewTexCoords);

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				Position.x += 16.0f;
				currentData++;
			}
			Position.y -= 16.0f;
			Position.x = StartingX;
		}
		
		/*
		{
			ImGui::ColorEdit4("square color", textureColor.rgba);
			ImGui::ColorEdit4("clear color", m_OrtographicCamera->CameraBackgroundColor.rgba);
			ImGui::SliderFloat("Sprite Rotation", &SpriteRotation, -180.0f, 180.0f);
		}
		*/
		
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