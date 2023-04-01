#pragma once

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"

namespace Systems
{
	// Archive
	void updateKeyboardInputsVelocity(entt::registry& registry, Window& window);


	// Updates entities which move according to keyboard input
	void updateKeyboardInputVelocities(entt::registry& registry, Window& window);

	// Updates velocity according to the values stored in acceleration
	void updateVelocities(entt::registry& registry, Window& window);

	// Caps velocity to MaxSpeed for entities with a MaxSpeed
	void capVelocities(entt::registry& registry);

	// Update enemy movement with velocity
	void updateMovement(entt::registry& registry, Window& window);
}