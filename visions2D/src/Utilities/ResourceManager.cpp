#include "ResourceManager.h"
#include "Log.h"

#include "Renderer/Texture.h"
#include "UI/Font.h"

namespace visions2D {

	ResourceManager::ResourceManager() {}
	ResourceManager::~ResourceManager() {
		ClearData();
	}

	void ResourceManager::ClearData() {
		// TODO: Do I have to iterate and delete all pointers? possibly!
		m_Textures.clear();
		m_Fonts.clear();
	}

	void ResourceManager::AddTexture(const std::string& TextureID, const std::string& filepath) {
		Texture* tex = new Texture();
		if (tex->Load(filepath)) {
			m_Textures.emplace(TextureID, tex);
		}
		else {
			LOG_ERROR("Couldn't load texture {0}", filepath);
		}
	}

	void ResourceManager::RemoveTexture(const std::string& TextureID) {
		if (m_Textures.count(TextureID) == 0) {
			LOG_WARNING("There's no texture with ID {0}", TextureID);
			return;
		}

		Texture* tex = m_Textures[TextureID];
		tex->Unload();
		delete tex;
		m_Textures.erase(TextureID);
	}

	void ResourceManager::AddFont(const std::string& FontID, const std::string& filepath) {
		Font* f = new Font();
		if (f->Load(filepath)) {
			m_Fonts.emplace(FontID, f);
		}
		else {
			LOG_ERROR("Couldn't load font {0}", filepath);
		}
	}

	void ResourceManager::RemoveFont(const std::string& FontID) {
		if (m_Fonts.count(FontID) == 0) {
			LOG_WARNING("There's no font with ID {0}", FontID);
			return;
		}

		m_Fonts.erase(FontID);
	}

	void ResourceManager::CreateTextureFromFont(const std::string& TextureID, Font* font, const std::string& characters, int size) {
		Texture* tex = new Texture();
		tex = font->RenderToTexture(characters, size);
		m_Textures.emplace(TextureID, tex);
	}

	void ResourceManager::CreateTextureFromFont(const std::string& TextureID, const std::string& font, const std::string& characters, int size) {
		Font* f = GetFont(font);
		
		if (f != nullptr) {
			CreateTextureFromFont(TextureID, f, characters, size);
		}
		else {
			LOG_ERROR("Couldn't find font: {0}", font);
		}
	}

	Texture* ResourceManager::GetTexture(const std::string& TextureID) {
		if (m_Textures.count(TextureID) == 0) {
			LOG_ERROR("Couldn't find texture with ID: {0}", TextureID);
			return nullptr;
		}

		return m_Textures[TextureID];
	}

	Font* ResourceManager::GetFont(const std::string& FontID) {
		if (m_Fonts.count(FontID) == 0) {
			LOG_ERROR("Couldn't find a font with ID {0}", FontID);
			return nullptr;
		}

		return m_Fonts[FontID];
	}
}