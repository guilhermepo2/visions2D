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

	static const int OPENGL_MAJOR_VERSION = 4;
	static const int OPENGL_MINOR_VERSION = 5;
	static int Renderer_Stats_DrawCalls = 0;

	float DefaultTexCoords[] = {
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f
	};

	// ***********************************************************************************************************
	Renderer::Renderer() {}
	Renderer::~Renderer() {}

	// ***********************************************************************************************************
	bool Renderer::Initialize(float _ScreenWidth, float _ScreenHeight, const std::string& _WindowTitle) {
		
		SDL_Init(SDL_INIT_EVERYTHING);

		// this should be on some sort of renderer backend
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

		m_Window = new SDL2Window(
			static_cast<unsigned int>(_ScreenWidth), 
			static_cast<unsigned int>(_ScreenHeight),
			_WindowTitle
		);

		// Initializing Fonts
		if (TTF_Init() != 0) {
			LOG_ERROR("Failed to initialize SDL_ttf!");
			return false;
		}

		m_OrtographicCamera = new OrtographicCamera(
			static_cast<float>(m_Window->GetScreenWidth()),
			static_cast<float>(m_Window->GetScreenHeight())
		);
		m_OrtographicCamera->SetColor(0.0f, 0.0f, 0.0f, 1.0f);

		// TODO: Not generic
		m_SpriteShader = new Shader();
#if visions2D_DEBUG
		m_SpriteShader->Load("./src/assets/Shaders/DefaultSprite.vert", "./src/assets/Shaders/DefaultSprite.frag");
#else
		m_SpriteShader->LoadFromProgramString(DefaultSpriteVertexShader, DefaultSpriteFragmentShader);
#endif

		#include "DefaultVertexArray.data"
		m_DefaultVertexArray = new VertexArray(vertices, 4, 8, texCoords, indices, 6);

		// TODO: should every application initialize DearImGui? maybe this should be an option?
		DearImGui::Initialize(
			static_cast<SDL_Window*>(m_Window->GetPlatformSpecificWindow()), 
			(static_cast<SDL2Window*>(m_Window))->GetGLContext()
		);

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

	// ***********************************************************************************************************
	// ***********************************************************************************************************
	void Renderer::PrepareToRender() {
		
		DearImGui::BeginRender(
			static_cast<SDL_Window*>(m_Window->GetPlatformSpecificWindow())
		);

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

		// Activating the Sprite Shader and setting Camera View Projection is universal to the render pass, doesn't need to be changed for every quad
		m_SpriteShader->SetActive();
		m_SpriteShader->SetMatrix4("uCameraViewProjection", m_OrtographicCamera->GetCameraViewProjection());
	}

	// ***********************************************************************************************************
	// ***********************************************************************************************************
	void Renderer::DrawQuad(glm::vec2 Position, glm::vec2 TexSize, Color Tint) {
		RenderData r;

		r.WorldPosition = Position;
		r.TextureScale = TexSize;
		r.tint = Tint;

		this->m_SpriteRenderData.push_back(r);
	}

	void Renderer::DrawQuad(glm::vec2 Position) {
		Renderer::DrawQuad(
			Position,
			glm::vec2(WhiteTexture->GetWidth(), WhiteTexture->GetHeight()),
			Color::WHITE
		);
	}

	void Renderer::DrawQuad(float x, float y) { DrawQuad(glm::vec2(x, y)); }
	void Renderer::DrawQuad(RenderData rd) { this->m_SpriteRenderData.push_back(rd); }

	// ***********************************************************************************************************
	// ***********************************************************************************************************
	void Renderer::Render() {
		Renderer_Stats_DrawCalls = 0;

		for (int i = 0; i < m_SpriteRenderData.size(); i++) {

			if (m_SpriteRenderData[i].Texture != nullptr) {
				m_SpriteRenderData[i].Texture->SetActive();
			}
			else {
				WhiteTexture->SetActive();
			}

			glm::mat4 TextureSize = glm::scale(glm::mat4(1.0f), glm::vec3(m_SpriteRenderData[i].TextureScale, 1.0f));
			glm::mat4 WorldScale = glm::scale(glm::mat4(1.0f), glm::vec3(m_SpriteRenderData[i].WorldScale, 1.0f));
			glm::mat4 WorldRotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_SpriteRenderData[i].WorldRotation), glm::vec3(0.0f, 0.0f, -1.0f));
			glm::mat4 WorldTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(m_SpriteRenderData[i].WorldPosition, 0.0f));
			glm::mat4 World = (WorldTranslation * WorldRotation * WorldScale) * TextureSize;

			// TODO: needs a function to create any four vertex from x and y position
			// calculating the four vertex positions
			glm::vec4 a = World * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
			glm::vec4 b = World * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
			glm::vec4 c = World * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
			glm::vec4 d = World * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);

			float VertexPos[] = {
				a.x, a.y,
				b.x, b.y,
				c.x, c.y,
				d.x, d.y
			};

			m_DefaultVertexArray->SetActive();
			m_DefaultVertexArray->SubPosCoords(VertexPos);
			m_DefaultVertexArray->SubColorCoords(m_SpriteRenderData[i].tint);

			if (m_SpriteRenderData[i].TexCoords == nullptr) {
				m_DefaultVertexArray->SubTexCoords(DefaultTexCoords);
			}
			else {
				m_DefaultVertexArray->SubTexCoords(m_SpriteRenderData[i].TexCoords);
			}

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			Renderer_Stats_DrawCalls += 1;
		}

		m_SpriteRenderData.clear();
	}

	// ***********************************************************************************************************
	// ***********************************************************************************************************
	void Renderer::Swap() {
		DearImGui::Present();
		m_Window->Swap();
	}

	// ***********************************************************************************************************
	// ***********************************************************************************************************
	void Renderer::Shutdown() {
		LOG_INFO("[renderer] shutting down");
		delete m_Window;
		SDL_Quit();
	}

	// ***********************************************************************************************************
	// ***********************************************************************************************************
	int Renderer::GetDrawCalls() const {
		return Renderer_Stats_DrawCalls;
	}
}