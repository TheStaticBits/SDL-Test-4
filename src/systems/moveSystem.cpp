#include "systems/moveSystem.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/movement.h"
#include "comps/position.h"

namespace Systems
{
	void updateVelocity(entt::registry& registry, Window& window)
	{
		auto view = registry.view<Comps::Movement, Tags::KeyboardInput>();
		
		for (const entt::entity entity : view)
		{
			auto& movement = view.get<Comps::Movement>(entity);

			//// Updates velocity with acceleration based on input
			movement.vel.x += (window.getKeyState(SDLK_RIGHT) - window.getKeyState(SDLK_LEFT)) * movement.acceleration * window.getDeltaTime();
			movement.vel.y += (window.getKeyState(SDLK_DOWN) - window.getKeyState(SDLK_UP)) * movement.acceleration * window.getDeltaTime();
		}
	}

	void movement(entt::registry& registry, Window& window)
	{
		auto view = registry.view<Comps::Movement, Comps::Position>();

		for (const entt::entity entity : view)
		{
			auto [movement, position] = view.get<Comps::Movement, Comps::Position>(entity);
			position.pos += movement.vel;
		}
	}
}