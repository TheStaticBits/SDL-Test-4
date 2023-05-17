#pragma once

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <tuple>
#include <optional>

#include "core/window.h"
#include "comps/position.h"
#include "comps/hitbox.h"

namespace Systems
{
	// Updates entities which move according to keyboard input
	void updateKeyboardInputVels(entt::registry& registry, Window& window);
	// Updates one direction 
	void updateKeyboardAcc(Window& window, const int& keyInputDir, float& vel, float& acc, float& staticAcc);

	// Updates entities affected by gravity
	void updateGravity(entt::registry& registry);

	// Updates velocity according to the values stored in acceleration
	void updateVelocities(entt::registry& registry, Window& window);
	// Caps velocity to MaxSpeed for entities with a MaxSpeed
	void capVelocities(entt::registry& registry);

	// Update enemy movement with velocity
	void updateMovement(entt::registry& registry, Window& window);
	void updateMovWithCollisions(entt::registry& registry, Window& window);

	// Moves the position to the edge of the hitboxes after a collision is detected
	void moveToEdge(entt::registry& registry, float& vel, float& pos,
				    const float parentPos,    const uint32_t parentSize,
				    const float collidedPos,  const uint32_t collidedSize);

	// Returns the entity collided with and both hitboxes that collided
	const std::optional<std::tuple<entt::entity, Comps::Box, Comps::Box>>
	checkAllCollisions(entt::registry& registry, Comps::Position pos, Comps::Hitbox hitbox, const entt::entity& entity);
}