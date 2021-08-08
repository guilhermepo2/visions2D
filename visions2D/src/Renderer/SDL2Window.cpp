#include "SDL2Window.h"
#include "Log.h"

namespace visions2D {
	SDL2Window::SDL2Window(unsigned int WindowWidth, unsigned int WindowHeight, std::string WindowTitle) {
		unsigned int WasInitialized = SDL_WasInit(SDL_INIT_EVERYTHING);
		m_ScreenWidth = WindowWidth;
		m_ScreenHeight = WindowHeight;
		m_WindowTitle = WindowTitle;

		assert(WasInitialized, "SDL was not initialized before creating a window?!");

		m_Window = SDL_CreateWindow(
			WindowTitle.c_str(), 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 
			WindowWidth, 
			WindowHeight, 
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);

		assert(m_Window, "[sdl2window] unable to create window: {0}", SDL_GetError());

		m_GLContext = SDL_GL_CreateContext(m_Window);
		SDL_GL_MakeCurrent(m_Window, m_GLContext);
		SDL_GL_SetSwapInterval(1);

		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			LOG_ERROR("[renderer] unable to initialize glad!");
		}

		glGetError();
		glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);

		LOG_INFO("SDL2 Window was created!")
	}

	void SDL2Window::Swap() {
		SDL_GL_SwapWindow(m_Window);
	}

	SDL2Window::~SDL2Window() {
		Destroy();
	}

	void SDL2Window::Destroy() {
		SDL_GL_DeleteContext(m_GLContext);
		SDL_DestroyWindow(m_Window);
	}
}