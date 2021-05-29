#pragma once
#include <string>

struct SDL_Surface;

namespace visions2D {
	class Texture {
	public:
		Texture();
		~Texture();

		bool Load(const std::string& _fileName);
		void Unload();
		void SetActive();
		void CreateFromSurface(SDL_Surface* Surface);

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }
		
	private:
		unsigned int m_TextureID;
		int m_Width;
		int m_Height;
	};
}