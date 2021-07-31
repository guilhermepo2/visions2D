#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Color.h"
#include "OrtographicCamera.h"
#include "Tilesheet.h"
#include "DearImGui.h"
#include "Framebuffer.h"

#include <Log.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Utilities/Tilemap.h"



namespace visions2D {

	// the image is 16x16, and we have 4 fields for each pixel (rgba)
	static const int TEX_SIZE_16_BY_16 = (16 * 16 * 4);

	// TODO: Make a tool that creates these strings from a shader file...
	static const char* DefaultSpriteFragmentShader = "#version 330 core\nout vec4 outColor;\nin vec2 TexCoord;\nuniform sampler2D textureSampler;\nuniform vec4 uColor;\nvoid main()\n{\noutColor = uColor * texture(textureSampler, TexCoord);\n}";
	static const char* DefaultSpriteVertexShader = "#version 330 core\nlayout(location = 0) in vec2 aPos;\nlayout(location = 1) in vec2 aTexCoord;\nuniform mat4 uWorldTransform;\nuniform mat4 uCameraViewProjection;\nout vec2 TexCoord;\nvoid main()\n{\nvec4 pos = vec4(aPos, 0.0, 1.0);\ngl_Position = uCameraViewProjection * uWorldTransform * pos;\nTexCoord = aTexCoord;\n}";

	static unsigned char WhiteTextureArray[TEX_SIZE_16_BY_16];

	static const int OPENGL_MAJOR_VERSION = 3;
	static const int OPENGL_MINOR_VERSION = 3;

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
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		m_Window = SDL_CreateWindow(
			m_WindowTitle.c_str(),
			100, 100,
			static_cast<int>(m_ScreenWidth), static_cast<int>(m_ScreenHeight),
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);

		assert(m_Window, "[renderer] unable to create window: {0}", SDL_GetError());

		m_GLContext = SDL_GL_CreateContext(m_Window);
		SDL_GL_MakeCurrent(m_Window, m_GLContext);
		SDL_GL_SetSwapInterval(1);

		glewExperimental = GL_TRUE;
		assert(glewInit() == GLEW_OK, "[renderer] unable to initialize glew");
		glGetError();
		glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);

		// Initializing Fonts
		if (TTF_Init() != 0) {
			LOG_ERROR("Failed to initialize SDL_ttf!");
			return false;
		}

		m_OrtographicCamera = new OrtographicCamera(m_ScreenWidth, m_ScreenHeight);
		m_OrtographicCamera->SetColor(0.0f, 0.0f, 0.0f, 1.0f);

		// TODO: Not generic
		m_SpriteShader = new Shader();
		m_SpriteShader->LoadFromProgramString(DefaultSpriteVertexShader, DefaultSpriteFragmentShader);

		#include "DefaultVertexArray.data"
		m_DefaultVertexArray = new VertexArray(vertices, 4, 4, texCoords, indices, 6);

		// TODO: should every application initialize DearImGui? maybe this should be an option?
		DearImGui::Initialize(m_Window, m_GLContext);
		LOG_INFO("[renderer] dearimgui initialized");

		// Initializing some common components maybe?
		memset(WhiteTextureArray, 255, TEX_SIZE_16_BY_16);
		WhiteTexture = new Texture();
		WhiteTexture->CreateFromArray(WhiteTextureArray, 16, 16);

		LOG_INFO("[renderer] OpenGL version: {0}", glGetString(GL_VERSION));
		LOG_INFO("[renderer] OpenGL vendor: {0}", glGetString(GL_VENDOR));
		LOG_INFO("[renderer] OpenGL renderer: {0}", glGetString(GL_RENDERER));
		LOG_INFO("[renderer] initialized!");
		return true;
	}

	void Renderer::PrepareToRender() {
		
		DearImGui::BeginRender(m_Window);

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
	}

	// TODO: Batch
	// How do colors work in batching?
	void Renderer::Render() {
		Uint32 TicksAtBeginning = SDL_GetTicks();
		for (int i = 0; i < SpriteRenderData.size(); i++) {
			m_SpriteShader->SetActive();
			m_SpriteShader->SetColor("uColor", SpriteRenderData[i].tint);

			if (SpriteRenderData[i].Texture != nullptr) {
				SpriteRenderData[i].Texture->SetActive();
			}
			else {
				WhiteTexture->SetActive();
			}

			glm::mat4 TextureScale = glm::scale(glm::mat4(1.0f), glm::vec3(SpriteRenderData[i].TextureScale, 1.0f));
			glm::mat4 WorldScale = glm::scale(glm::mat4(1.0f), glm::vec3(SpriteRenderData[i].WorldScale, 1.0f));
			glm::mat4 WorldRotation = glm::rotate(glm::mat4(1.0f), glm::radians(SpriteRenderData[i].WorldRotation), glm::vec3(0.0f, 0.0f, -1.0f));
			glm::mat4 WorldTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(SpriteRenderData[i].WorldPosition, 0.0f));
			glm::mat4 World = (WorldTranslation * WorldRotation * WorldScale) * TextureScale;

			m_SpriteShader->SetMatrix4("uWorldTransform", World);
			m_SpriteShader->SetMatrix4("uCameraViewProjection", m_OrtographicCamera->GetCameraViewProjection());

			m_DefaultVertexArray->SetActive();
			if (SpriteRenderData[i].TexCoords == nullptr) {
				m_DefaultVertexArray->SubTexCoords(DefaultTexCoords);
			}
			else {
				m_DefaultVertexArray->SubTexCoords(SpriteRenderData[i].TexCoords);
			}

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		SpriteRenderData.clear();
	}

	void Renderer::Swap() {
		DearImGui::Present();
		SDL_GL_SwapWindow(m_Window);
	}

	void Renderer::Shutdown() {
		LOG_INFO("[renderer] shutting down");
		SDL_GL_DeleteContext(m_GLContext);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}
}