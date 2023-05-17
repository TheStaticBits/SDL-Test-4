#include "systems/moveSystem.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <tuple>
#include <optional>

#include "core/window.h"
#include "comps/movement.h"
#include "comps/position.h"
#include "comps/hitbox.h"

#include "utility/collision.h"

namespace Systems
{
	void updateKeyboardInputVels(entt::registry& registry, Window& window)
	{
		const int dirX = (window.getKeyState(SDLK_RIGHT) - window.getKeyState(SDLK_LEFT));
		const int dirY = (window.getKeyState(SDLK_DOWN) - window.getKeyState(SDLK_UP));

		const auto view = registry.view<Comps::Movement, Comps::KeyboardInput>();
		for (const entt::entity entity : view)
		{
			auto [movement, keyboardInput] = view.get<Comps::Movement, Comps::KeyboardInput>(entity);
			
			// Update x and y accelerations
			updateKeyboardAcc(window, dirX, movement.vel.x, movement.acc.x, keyboardInput.staticAcc.x);
			updateKeyboardAcc(window, dirY, movement.vel.y, movement.acc.y, keyboardInput.staticAcc.y);
		}
	}

	void updateKeyboardAcc(Window& window, const int& keyInputDir, float& vel, float& acc, float& staticAcc)
	{
		if (keyInputDir != 0) 
			acc = staticAcc * keyInputDir; // Accelerate in the direction of the input
		else if (vel != 0) // No key press but still moving
		{
			acc = (vel < 0 ? 1 : -1) * staticAcc; // Slowing down

			if (vel <= acc * window.getDeltaTime() && vel >= -acc * window.getDeltaTime())
			{
				vel = 0.0f; // Near enough to zero to round to zero
				acc = 0.0f;
			}
		}
	}

	void updateGravity(entt::registry& registry)
	{
		const auto view = registry.view<Comps::Gravity, Comps::Movement>();
		for (const entt::entity entity : view)
		{
			auto [gravity, movement] = view.get<Comps::Gravity, Comps::Movement>(entity);
			movement.acc.y = gravity.strength;
		}
	}

	void updateVelocities(entt::registry& registry, Window& window)
	{
		const auto view = registry.view<Comps::Movement>();
		for (const entt::entity entity : view)
		{
			auto& movement = view.get<Comps::Movement>(entity);
			// Accelerate
			movement.vel += movement.acc * window.getDeltaTime();
		}
	}

	void capVelocities(entt::registry& registry)
	{
		const auto view = registry.view<Comps::Movement, Comps::MaxSpeed>();
		for (const entt::entity entity : view)
		{
			auto [mov, max] = view.get<Comps::Movement, Comps::MaxSpeed>(entity);

			// Capping at max speed
			if (mov.vel.x > max.speed.x)        mov.vel.x = max.speed.x;
			else if (mov.vel.x < -max.speed.x)  mov.vel.x = -max.speed.x;

			if (mov.vel.y > max.speed.y) mov.vel.y = max.speed.y;
			else if (mov.vel.y < -max.speed.y)  mov.vel.y = -max.speed.y;
		}
	}

	void updateMovement(entt::registry& registry, Window& window)
	{
		const auto view = registry.view<Comps::Movement, Comps::Position>();
		for (const entt::entity entity : view)
		{
			if (registry.all_of<Tags::ChecksCollisions>(entity))
				continue;

			auto [movement, position] = view.get<Comps::Movement, Comps::Position>(entity);
			position.pos += movement.vel * window.getDeltaTime();
		}
	}

	// Updates objects which check for collision
	void updateMovWithCollisions(entt::registry& registry, Window& window)
	{
		const auto view = registry.view<Comps::Movement, Comps::Position, Comps::Hitbox, Tags::ChecksCollisions>();
		for (const entt::entity entity : view)
		{
			auto [mov, pos, hitbox] = view.get<Comps::Movement, Comps::Position, Comps::Hitbox>(entity);

			pos.pos.x += mov.vel.x * window.getDeltaTime();

			// Check collisions from x movement
			auto collisionCheckX = Systems::checkAllCollisions(registry, pos, hitbox, entity);
			if (collisionCheckX.has_value())
			{
				const auto [collisionEntityX, thisBoxX, collidedBoxX] = collisionCheckX.value();

				Systems::moveToEdge(registry, mov.vel.x, pos.pos.x,
									thisBoxX.offset.offset.x, thisBoxX.size.x,
									collidedBoxX.offset.offset.x, collidedBoxX.size.x);
			}
				
			pos.pos.y += mov.vel.y * window.getDeltaTime();

			// Check collisions from y movement
			auto collisionCheckY = Systems::checkAllCollisions(registry, pos, hitbox, entity);
			if (collisionCheckY.has_value())
			{
				const auto [collisionEntityY, thisBoxY, collidedBoxY] = collisionCheckY.value();
				
				//if (mov.vel.y > 0)
				//{
				//	registry.destroy(collisionEntityY);
				//	acc.acc.y = -200;
				//}

				Systems::moveToEdge(registry, mov.vel.y, pos.pos.y,
									thisBoxY.offset.offset.y, thisBoxY.size.y,
									collidedBoxY.offset.offset.y, collidedBoxY.size.y);
			}
		}
	}

	// Locks the object's position to either the top of the collided object or the bottom depending on velocity
	// Used for both directions
	void moveToEdge(entt::registry& registry, float& vel, float& pos,
				    const float parentPos,    const uint32_t parentSize,
				    const float collidedPos,  const uint32_t collidedSize)
	{
		float distance = 0.0f;

		if (vel < 0) // Moving up/left, find distance between bottom of collided and top of parent box
			distance = parentPos - (collidedPos + static_cast<float>(collidedSize));
		else if (vel > 0) // Moving down/right, distance between bottom of parent and top of collided
			distance = collidedPos - (parentPos + static_cast<float>(parentSize));

		// Moving to the edge of the hitbox
		pos += distance;
		vel = 0;
	}

	// Returns the first element that is found to collide with the entity, and its position & size 
	// Or returns the entity passed in as an arguement if no entities collided were found
	const std::optional<std::tuple<entt::entity, Comps::Box, Comps::Box>>
	checkAllCollisions(entt::registry& registry, Comps::Position pos, Comps::Hitbox hitbox, const entt::entity& entity)
	{
		const auto view = registry.view<Comps::Position, Comps::Hitbox>();
		for (const entt::entity checkingEntity : view)
		{
			if (checkingEntity == entity)
				continue;

			const auto [checkingPos, checkingHitbox] = view.get<Comps::Position, Comps::Hitbox>(checkingEntity);

			// For each box in the object's hitbox, check it with all tile hitboxes
			// Can be optimized much more...
			// Also means that the player can have more than one hitbox probably
			for (Comps::Box parentHitboxBox : hitbox.boxes)
			{
				// Adding position to the hitbox's offset to find its onscreen position
				parentHitboxBox.offset.offset += pos.pos;

				// Each hitbox of the entity being checked
				for (Comps::Box checkingHitboxBox : checkingHitbox.boxes)
				{
					checkingHitboxBox.offset.offset += checkingPos.pos;

					if (Utility::collision(parentHitboxBox, checkingHitboxBox))
						return std::tuple<entt::entity, Comps::Box, Comps::Box>{ checkingEntity, parentHitboxBox, checkingHitboxBox };
				}
			}
		}

		return {}; // No entity collision found
	}
}