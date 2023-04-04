#pragma once

#include <entt/entt.hpp>

#include "utility/vect.h"

namespace Systems
{
	void updateCameraMovement(entt::registry& registry);

	const Vect<int32_t> getPlayerPos(entt::registry& registry);
}