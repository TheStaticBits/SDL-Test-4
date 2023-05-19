#pragma once

#include <vector>
#include <entt/entt.hpp>
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

	// Callbacks for when a collision occurs on both x and y axis
	struct Collision
	{
		Vect<std::function<void(entt::registry& registry, entt::entity entity, entt::entity collidedEntity, float& velocity)>> callbacks;
	};
}