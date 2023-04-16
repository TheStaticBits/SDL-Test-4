#include "systems/moveSystem.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/movement.h"
#include "comps/position.h"

namespace Systems
{
	void updateKeyboardInputVelocities(entt::registry& registry, Window& window)
	{
		const int dirX = (window.getKeyState(SDLK_RIGHT) - window.getKeyState(SDLK_LEFT));
		const int dirY = (window.getKeyState(SDLK_DOWN) - window.getKeyState(SDLK_UP));

		auto view = registry.view<Comps::Movement, Comps::Acceleration, Comps::StaticAcceleration, Tags::KeyboardInput>();
		for (const entt::entity entity : view)
		{
			auto [mov, acceleration, staticAcc] = view.get<Comps::Movement, Comps::Acceleration, Comps::StaticAcceleration>(entity);
			
			if (dirX != 0) acceleration.acc.x = staticAcc.acc.x * dirX;
			else if (mov.vel.x != 0.0f)
			{
				acceleration.acc.x = (mov.vel.x < 0 ? 1 : -1) * staticAcc.acc.x;
				if (mov.vel.x <= acceleration.acc.x * window.getDeltaTime() && mov.vel.x >= -acceleration.acc.x * window.getDeltaTime())
				{
					mov.vel.x = 0.0f; // Near enough to zero to round to zero
					acceleration.acc.x = 0.0f;
				}
			}


			if (dirY != 0) acceleration.acc.y = staticAcc.acc.y * dirY;
			else if (mov.vel.y != 0)
			{
				acceleration.acc.y = (mov.vel.y < 0 ? 1 : -1) * staticAcc.acc.y;
				if (mov.vel.y <= acceleration.acc.y * window.getDeltaTime() && mov.vel.y >= -acceleration.acc.y * window.getDeltaTime())
				{
					mov.vel.y = 0.0f;
					acceleration.acc.y = 0.0f;
				}
			}
		}
	}

	void updateVelocities(entt::registry& registry, Window& window)
	{
		auto view = registry.view<Comps::Movement, Comps::Acceleration>();
		for (const entt::entity entity : view)
		{
			auto [mov, acceleration] = view.get<Comps::Movement, Comps::Acceleration>(entity);
			// Accelerate
			mov.vel += acceleration.acc * window.getDeltaTime();
		}
	}

	void capVelocities(entt::registry& registry)
	{
		auto view = registry.view<Comps::Movement, Comps::MaxSpeed>();
		for (const entt::entity entity : view)
		{
			auto [mov, max] = view.get<Comps::Movement, Comps::MaxSpeed>(entity);

			// Capping at max speed
			if (mov.vel.x > max.maxSpeed)        mov.vel.x = max.maxSpeed;
			else if (mov.vel.x < -max.maxSpeed)  mov.vel.x = -max.maxSpeed;

			if (mov.vel.y > max.maxSpeed) mov.vel.y = max.maxSpeed;
			else if (mov.vel.y < -max.maxSpeed)  mov.vel.y = -max.maxSpeed;
		}
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