#pragma once
#include <string>

namespace visions2D {
	class Tilemap {
	public:
		Tilemap();
		~Tilemap();
		void LoadFromJSON(const std::string& _filename);

		int GetData(int index);
		inline int GetMapWidth() const { return m_MapWidth; }
		inline int GetMapHeight() const { return m_MapHeight; }

	private:
		int* m_MapData;
		int m_MapSize;

		int m_MapWidth;
		int m_MapHeight;
	};
}