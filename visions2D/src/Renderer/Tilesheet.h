#pragma once
#include <string>
#include <rapidjson/document.h>

namespace visions2D {
	
	class Texture;
	class Sprite;

	class Tilesheet {
	public:
		Tilesheet(Texture* _texture);
		void LoadFromTiledJson(const std::string& _fileName);
		float* GetTexCoordsFromPosition(int x, int y);
		float* GetTexCoordsFromId(int id);
		

		inline int GetTileWidth() const { return m_TileWidth; }
		inline int GetTileHeight() const { return m_TileHeight; }
		inline int GetNumberOfColumns() const { return m_NumberOfColumns; }
		inline int GetNumberOfVerticalTiles() const { return m_NumberOfVerticalTiles; }

	private:
		bool LoadJson(const std::string& FileName, rapidjson::Document& OutDocument);

	private:
		Texture* m_TextureRef;
		int m_NumberOfColumns;
		int m_NumberOfVerticalTiles;
		int m_TileCount;
		int m_TileWidth;
		int m_TileHeight;
	};
}