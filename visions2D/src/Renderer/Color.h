#pragma once

namespace visions2D {
	class Color {
	public:
		float rgba[4];

		Color() {
			rgba[0] = 1.0f;
			rgba[1] = 1.0f;
			rgba[2] = 1.0f;
			rgba[3] = 1.0f;
		}

		Color(float _r, float _g, float _b, float _a) {
			rgba[0] = _r;
			rgba[1] = _g;
			rgba[2] = _b;
			rgba[3] = _a;
		}
	};
}