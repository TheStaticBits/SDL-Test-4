#pragma once

#include <entt/entt.hpp>

#include "utility/vect.h"

namespace Systems
{
	void updateCameraMovement(entt::registry& registry);

	const Vect<float> getPlayerPos(entt::registry& registry);
	const Vect<uint32_t> getPlayerTexSize(entt::registry& registry);
}