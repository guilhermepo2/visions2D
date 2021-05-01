#include "Tilesheet.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <fstream>
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

		if (!LoadJson(_fileName, doc)) {
			LOG_ERROR("[tilesheet] loading tilesheet: {0}", _fileName.c_str());
			return;
		}

		LOG_INFO("[tilesheet] parsing tilesheet: {0}", _fileName.c_str());

		JsonHelper::GetInt(doc, "columns", m_NumberOfColumns);
		JsonHelper::GetInt(doc, "tilecount", m_TileCount);
		JsonHelper::GetInt(doc, "tilewidth", m_TileWidth);
		JsonHelper::GetInt(doc, "tileheight", m_TileHeight);

		m_NumberOfVerticalTiles = (m_TextureRef->GetHeight() / m_TileHeight) - 1;
	}

	bool Tilesheet::LoadJson(const std::string& FileName, rapidjson::Document& OutDocument) {
		std::ifstream File(FileName, std::ios::in | std::ios::binary | std::ios::ate);
		if (!File.is_open()) {
			LOG_ERROR("[tilesheet] file {0} not found", FileName.c_str());
			return false;
		}

		// Get the current position in stream buffer, which is the size of the file
		std::ifstream::pos_type FileSize = File.tellg();
		File.seekg(0, std::ios::beg);

		// Create a vector of size + 1 (for null terminator)
		std::vector<char> Bytes(static_cast<size_t>(FileSize) + 1);
		// Read int bytes into vector
		File.read(Bytes.data(), static_cast<size_t>(FileSize));

		// Loading raw data into RapidJSON document
		OutDocument.Parse(Bytes.data());
		if (!OutDocument.IsObject()) {
			LOG_ERROR("[tilesheet] File {0} is not a valid JSON!", FileName.c_str());
			return false;
		}

		return true;
	}

	// TODO
	// this is bad
	// maybe when loading a new tilesheet make an array of Tiles?
	// and each tile has their own TexCoords
	// I CANNOT be allocating stuff here lol
	float* Tilesheet::GetTexCoordsFromPosition(int x, int y) {
		float tw = (float)m_TileWidth / m_TextureRef->GetWidth();
		float th = (float)m_TileHeight / m_TextureRef->GetHeight();

		float* NewTexCoords = new float[8];
		NewTexCoords[0] = (x + 1) * tw;
		NewTexCoords[1] = (y + 1) * th;

		NewTexCoords[2] = (x + 1) * tw;
		NewTexCoords[3] = y * th;

		NewTexCoords[4] = x * tw;
		NewTexCoords[5] = y * th;

		NewTexCoords[6] = x * tw;
		NewTexCoords[7] = (y + 1) * th;

		// the memory leak lmao
		return NewTexCoords;
	}

	float* Tilesheet::GetTexCoordsFromId(int id) {
		return GetTexCoordsFromPosition(
			((id % m_NumberOfColumns) - 1),
			m_NumberOfVerticalTiles - (id / m_NumberOfColumns)
		);
	}
}