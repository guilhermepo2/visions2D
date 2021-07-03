#pragma once
#include <unordered_map>

namespace visions2D {
	
	class Texture;
	class Font;

	class ResourceManager {
	public:
		ResourceManager();
		~ResourceManager();

		void ClearData();
		void AddTexture(const std::string& TextureID, const std::string& filepath);
		void RemoveTexture(const std::string& TextureID);
		void AddFont(const std::string& FontID, const std::string& filepath);
		void RemoveFont(const std::string& FontID);

		void CreateTextureFromFont(const std::string& TextureID, Font* font, const std::string& characters, int size);
		void CreateTextureFromFont(const std::string& TextureID, const std::string& font, const std::string& characters, int size);

		Texture* GetTexture(const std::string& TextureID);
		Font* GetFont(const std::string& FontID);
	private:
		std::unordered_map<std::string, Texture*> m_Textures;
		std::unordered_map<std::string, Font*> m_Fonts;
	};

}