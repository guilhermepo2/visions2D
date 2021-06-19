#include "Font.h"
#include "Log.h"
#include "Renderer/Texture.h"

namespace visions2D {
	Font::Font() {}
	Font::~Font() {}

	bool Font::Load(const std::string& InFontFileName) {
		std::vector<int> FontSizes = {
			8, 9,
			10, 11, 12, 14, 16, 18,
			20, 22, 24, 26, 28,
			30, 32, 34, 36, 38,
			40, 42, 44, 46, 48,
			52, 56,
			60, 64, 68,
			72
		};

		for (int size : FontSizes) {
			TTF_Font* f = TTF_OpenFont(InFontFileName.c_str(), size);

			if (f == nullptr) {
				LOG_ERROR("Failed to load font {0} on size {1]", InFontFileName, size);
				return false;
			}

			m_FontData.emplace(size, f);
		}
	}

	void Font::Unload() {
		for (auto& f : m_FontData) {
			TTF_CloseFont(f.second);
		}
	}

	Texture* Font::RenderToTextureWrapped(const std::string& Text, int PointSize, int WrappedSize) {
		Texture* tex = nullptr;
		SDL_Color color = { 255, 255, 255, 255 };
		auto iter = m_FontData.find(PointSize);
		if (iter != m_FontData.end()) {
			TTF_Font* font = iter->second;

			SDL_Surface* surf = TTF_RenderUTF8_Blended_Wrapped(font, Text.c_str(), color, WrappedSize);

			if (surf != nullptr) {
				tex = new Texture();
				tex->CreateFromSurface(surf);
				SDL_FreeSurface(surf);
			}
		}

		return tex;
	}

	// TODO: Make the color optional here?!
	Texture* Font::RenderToTexture(const std::string& Text, int PointSize) {
		Texture* tex = nullptr;

		SDL_Color color;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		color.a = 255;

		auto iter = m_FontData.find(PointSize);
		if (iter != m_FontData.end()) {
			TTF_Font* font = iter->second;

			SDL_Surface* surf = TTF_RenderUTF8_Blended(font, Text.c_str(), color);

			if (surf != nullptr) {
				tex = new Texture();
				tex->CreateFromSurface(surf);
				SDL_FreeSurface(surf);
			}
		}
		else {
			LOG_WARNING("Point size {0} is not supported", PointSize);
		}

		return tex;
	}
}