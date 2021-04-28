#include "Renderer.h"
#include "Shader.h"
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
		// TODO: Create Default Sprite Vertex

		LOG_INFO("[renderer] initialized!");
		return true;
	}

	void Renderer::Render() {
		m_SpriteShader->SetActive();
		m_SpriteShader->SetVec4("uColor", 1.0f, 0.0f, 0.0f, 1.0f);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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

		// vertex array object
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// vertex buffer object -> sends data to the vertex shader
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copies data to the buffer

		// EBO
		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(m_Window);
	}

	void Renderer::Shutdown() {
		LOG_INFO("[renderer] shutting down");
		SDL_GL_DeleteContext(m_GLContext);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}
}