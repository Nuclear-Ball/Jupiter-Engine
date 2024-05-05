#pragma once
#include <vector>

namespace Cosmonator {
	enum class Locations : unsigned char {
		Surface,
		Atmosphere,
		Space,
		BossLevel
	};

	struct Vessel {

	};

	struct Region {
		struct Chunk {

		};
		std::vector<Vessel> vessels;
		Chunk map[128 * 32];
		bool exists[128 * 32] = { 0 };
		//std::vector<Chunk*> chunks;
	};

	//
}