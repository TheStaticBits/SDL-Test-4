#pragma once

#include <vector>
#include "utility/vect.h"
#include "comps/offset.h"

namespace Comps
{
	struct Box
	{
		Comps::Offset offset;
		Vect<uint32_t> size;
	};

	struct Hitbox
	{
		std::vector<Comps::Box> boxes;
	};
}

namespace Tags
{
	struct ChecksCollisions{};
}