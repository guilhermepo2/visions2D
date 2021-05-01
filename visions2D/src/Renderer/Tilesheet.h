#pragma once

namespace visions2D {
	
	class Texture;
	class Sprite;

	class Tilesheet {
	public:
		Tilesheet();

		Sprite GetSprite(int x, int y);
		Sprite GetSprite(int id);

	private:
		Texture* m_TextureRef;
		int m_NumberOfColumns;
		int m_TileCount;
		int m_tileWidth;
		int m_TileHeight;
	};
}