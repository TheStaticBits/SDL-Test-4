#pragma once

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"

namespace Systems
{
	// Updates entities which move according to keyboard input
	void updateVelocity(entt::registry& registry, Window& window);

	// Update enemy movement with velocity
	void movement(entt::registry& registry, Window& window);
}