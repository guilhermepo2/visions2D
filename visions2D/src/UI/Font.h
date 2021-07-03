#pragma once

#include <SDL_ttf.h>
#include <string>
#include <unordered_map>

namespace visions2D {

	class Texture;

	class Font {
	public:
		Font();
		~Font();

		bool Load(const std::string& InFontFileName);
		void Unload();
		// TODO: Maybe Text should be a key?
		Texture* Font::RenderToTextureWrapped(const std::string& Text, int PointSize, int WrappedSize);
		Texture* RenderToTexture(const std::string& Text, int PointSize = 16);

	private:
		std::unordered_map<int, TTF_Font*> m_FontData;
	};

}