#pragma once
#include <glad/glad.h>
#include <SDL.h>

#include "Window.h"

namespace visions2D {
	class SDL2Window : public Window {
	public:
		SDL2Window(unsigned int WindowWidth, unsigned int WindowHeight, std::string WindowTitle);
		virtual ~SDL2Window();

		virtual void Swap();
		virtual inline void* GetPlatformSpecificWindow() const { return m_Window; }
		SDL_GLContext GetGLContext() const { return m_GLContext; }
		virtual void Destroy() override;

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_GLContext;
	};
}