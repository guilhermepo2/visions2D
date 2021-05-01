#include "Tilesheet.h"
#include <vector>
#include "Log.h"
#include "Utilities/JsonHelper.h"
#include "Texture.h"

namespace visions2D {
	Tilesheet::Tilesheet(Texture* _texture) 
	: m_TextureRef(_texture) {
		
	}

	void Tilesheet::LoadFromTiledJson(const std::string& _fileName) {
		rapidjson::Document doc;

		if (!JsonHelper::LoadJSON(_fileName, doc)) {
			LOG_ERROR("[tilesheet] loading tilesheet: {0}", _fileName.c_str());
			return;
		}

		LOG_INFO("[tilesheet] parsing tilesheet: {0}", _fileName.c_str());

		JsonHelper::GetInt(doc, "columns", m_NumberOfColumns);
		JsonHelper::GetInt(doc, "tilecount", m_TileCount);
		JsonHelper::GetInt(doc, "tilewidth", m_TileWidth);
		JsonHelper::GetInt(doc, "tileheight", m_TileHeight);
		m_NumberOfVerticalTiles = (m_TextureRef->GetHeight() / m_TileHeight);

		for (int i = 0; i < m_NumberOfVerticalTiles; i++) {
			for (int j = (m_NumberOfColumns - 1); j >= 0; j--) {
				int x = j;
				int y = i;
				float tw = (float)m_TileWidth / m_TextureRef->GetWidth();
				float th = (float)m_TileHeight / m_TextureRef->GetHeight();
				glm::vec2 min = glm::vec2(x * tw, y * th);
				glm::vec2 max = glm::vec2((x + 1) * tw, (y + 1) * th);

				m_Tiles.insert(m_Tiles.begin(), new Tile(min, max));
			}
		}

		LOG_INFO("loaded tilesheet from {0}, had {1} tiles", _fileName.c_str(), m_Tiles.size());
	}

	float* Tilesheet::GetTexCoordsFromId(int id) {
		// #important
		// IMPORTANT thing to keep in mind: "id" on tiled goes from 1 to MaxTiles
		// but our indices goes from 0 to MaxTiles - 1...

		id -= 1;

		if (id < 0 || id > (m_Tiles.size() - 1)) {
			LOG_WARNING("trying to access position {0}, but tile vector only has {1} elements, returning first element", id, m_Tiles.size());
			id = 0;
		}

		return m_Tiles[id]->TexCoords;
	}
}