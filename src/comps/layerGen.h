#pragma once

#include <vector>
#include <string>

namespace Comps
{
	// Stores vectors used in generation to see what tile spots are taken up
	struct LayerGen
	{
		std::vector<bool> nextLayer;
		std::vector<bool> prevLayer;

		uint32_t startingY;
	};
}