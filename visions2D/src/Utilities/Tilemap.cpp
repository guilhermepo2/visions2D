#include "Tilemap.h"
#include "Log.h"
#include "Utilities/JsonHelper.h"

namespace visions2D {
	Tilemap::Tilemap() : m_MapData(nullptr), m_MapWidth(0), m_MapHeight(0) { }

	Tilemap::~Tilemap() {
		if (m_MapData != nullptr) {
			delete m_MapData;
		}
	}

	void Tilemap::LoadFromJSON(const std::string& _filename) {
		rapidjson::Document doc;
		if (!JsonHelper::LoadJSON(_filename, doc)) {
			LOG_ERROR("[tilemap] loading tilemap: {0}", _filename.c_str());
			return;
		}

		LOG_INFO("loading tilemap from {0}", _filename.c_str());

		// Getting layers
		// TODO: Support more than one layer
		const rapidjson::Value& layers = doc["layers"];
		if (layers.IsArray()) {
			const rapidjson::Value& Layer = layers[0];

			if (Layer.IsObject()) {
				const rapidjson::Value& Data = Layer["data"];

				if (Data.IsArray()) {
					m_MapData = new int[Data.Size()];
					m_MapSize = Data.Size();

					for (rapidjson::SizeType i = 0; i < Data.Size(); i++) {
						m_MapData[i] = Data[i].GetInt();
					}
				}
			}
		}

		JsonHelper::GetInt(doc, "width", m_MapWidth);
		JsonHelper::GetInt(doc, "height", m_MapHeight);
	}

	int Tilemap::GetData(int index) {
		if (index < 0 || index > m_MapSize) {
			return -1;
		}

		return m_MapData[index];
	}
}