#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include <Log.h>

namespace visions2D {
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

		m_SpriteShader = new Shader();
		m_SpriteShader->Load("./src/DefaultAssets/Shaders/DefaultSprite.vert", "./src/DefaultAssets/Shaders/DefaultSprite.frag");

		float vertices[] = {
			// positions  // texture coords
			 0.5f,  0.5f, 1.0f, 1.0f,   // top right
			 0.5f, -0.5f, 1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f, 1.0f    // top left 
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
		};

		m_DefaultVertexArray = new VertexArray(vertices, 4,  4, indices, 6);

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

	static float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	void Renderer::Render() {
		m_SpriteShader->SetActive();
		m_SpriteShader->SetVec4("uColor", color[0], color[1], color[2], color[3]);
		
		// dearimgui setting it up
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(m_Window);
		ImGui::NewFrame();

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		m_DefaultVertexArray->SetActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		{
			ImGui::ColorEdit4("square color", color);
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