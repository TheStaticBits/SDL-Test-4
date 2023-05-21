#pragma once

#include <vector>
#include <string>

namespace Comps
{
	// Stores vectors used in generation to see what tile spots are taken up
	struct LayerGen
	{
		std::vector<std::vector<bool>> layers;

		uint32_t yPos;
		float windowHeight;
	};
}

namespace Tags
{
	struct LayerGen {};
}