#include "Texture.h"
#include "Log.h"
#include <GL/glew.h>
#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace visions2D {
	Texture::Texture() : m_TextureID(0), m_Width(0), m_Height(0) { }
	Texture::~Texture() { }

	bool Texture::Load(const std::string& _fileName) {
		int channels = 0;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(_fileName.c_str(), &m_Width, &m_Height, &channels, 0);

		if (data == nullptr) {
			LOG_ERROR("[texture] failed to load image {0}", _fileName.c_str());
			return false;
		}

		int format = GL_RGB;
		if (channels == 4) {
			format = GL_RGBA;
		}

		glGenTextures(1, &m_TextureID);
		
		if (m_TextureID == 0) {
			LOG_ERROR("[renderer] failed to create texture {0}", _fileName);
		}

		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		return true;
	}

	void Texture::CreateFromSurface(SDL_Surface* Surface) {
		m_Width = Surface->w;
		m_Height = Surface->h;
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	}

	void Texture::Unload() {
		glDeleteTextures(1, &m_TextureID);
	}

	void Texture::SetActive() {
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}
}