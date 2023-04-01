#include "systems/moveSystem.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/movement.h"
#include "comps/position.h"

namespace Systems
{
	void updateKeyboardInputsVelocity(entt::registry& registry, Window& window)
	{
		const auto view = registry.view<Comps::Movement, Comps::Acceleration, Tags::KeyboardInput>();

		for (const entt::entity entity : view)
		{
			auto [mov, acc] = view.get<Comps::Movement, Comps::Acceleration>(entity);

			const int dirX = (window.getKeyState(SDLK_RIGHT) - window.getKeyState(SDLK_LEFT));
			const int dirY = (window.getKeyState(SDLK_DOWN) - window.getKeyState(SDLK_UP));

			if (dirX != 0)
			{
				mov.vel.x += dirX * acc.acceleration * window.getDeltaTime(); // Accelerate
			}
			else // Move towards zero
				movement.vel.x += (movement.vel.x < 0 ? 1 : -1) * movement.acceleration * window.getDeltaTime();

			// Lock between max speed
			if (movement.vel.x > movement.maxSpeed)        movement.vel.x = movement.maxSpeed;
			else if (movement.vel.x < -movement.maxSpeed)  movement.vel.x = -movement.maxSpeed;

			else if (dirX == 0 && movement.vel.x <= movement.acceleration * window.getDeltaTime() && movement.vel.x >= -movement.acceleration * window.getDeltaTime())
				movement.vel.x = 0.0f; // Near enough to zero to round to zero

			// same for y direction
			// if (dirY != 0)
			// 	movement.vel.y += dirY * movement.acceleration * window.getDeltaTime();
			// else
			//	movement.vel.y += (movement.vel.y < 0 ? 1 : -1) * movement.acceleration * window.getDeltaTime();
			//
			// if (movement.vel.y > movement.maxSpeed)        movement.vel.y = movement.maxSpeed;
			// else if (movement.vel.y < -movement.maxSpeed)  movement.vel.y = -movement.maxSpeed;
			//
			// else if (dirY == 0 && movement.vel.y <= movement.acceleration * window.getDeltaTime() && movement.vel.y >= -movement.acceleration * window.getDeltaTime())
			//	movement.vel.y = 0.0f;
		}
	}

	void updateKeyboardInputVelocities(entt::registry& registry, Window& window)
	{
		// todo
	}

	void updateVelocities(entt::registry& registry, Window& window)
	{
		// todo
	}

	void capVelocities(entt::registry& registry)
	{
		// todo
	}

	void updateMovement(entt::registry& registry, Window& window)
	{
		auto view = registry.view<Comps::Movement, Comps::Position>();

		for (const entt::entity entity : view)
		{
			auto [movement, position] = view.get<Comps::Movement, Comps::Position>(entity);
			position.pos += movement.vel * window.getDeltaTime();
		}
	}
}