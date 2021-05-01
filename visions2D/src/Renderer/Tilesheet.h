#pragma once
#include <string>
#include <rapidjson/document.h>
#include <vector>
#include <glm/glm.hpp>

namespace visions2D {
	
	class Texture;
	class Sprite;

	class Tile {
	public:
		Tile(const glm::vec2& min, const glm::vec2& max) {
			TexCoords = new float[8];

			TexCoords[0] = max.x;
			TexCoords[1] = max.y;

			TexCoords[2] = max.x;
			TexCoords[3] = min.y;

			TexCoords[4] = min.x;
			TexCoords[5] = min.y;

			TexCoords[6] = min.x;
			TexCoords[7] = max.y;
		}

		~Tile() {
			delete TexCoords;
		}

		float* TexCoords;
	};

	class Tilesheet {
	public:
		Tilesheet(Texture* _texture);
		// TODO: Destructor, have to delete all Tiles

		void LoadFromTiledJson(const std::string& _fileName);
		float* GetTexCoordsFromId(int id);

		inline int GetTileWidth() const { return m_TileWidth; }
		inline int GetTileHeight() const { return m_TileHeight; }
		inline int GetNumberOfColumns() const { return m_NumberOfColumns; }
		inline int GetNumberOfVerticalTiles() const { return m_NumberOfVerticalTiles; }

	private:
		bool LoadJson(const std::string& FileName, rapidjson::Document& OutDocument);

	private:
		Texture* m_TextureRef;
		std::vector<Tile*> m_Tiles;

		int m_NumberOfColumns;
		int m_NumberOfVerticalTiles;
		int m_TileCount;
		int m_TileWidth;
		int m_TileHeight;
	};
}