#include "systems/moveSystem.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/movement.h"
#include "comps/position.h"

namespace Systems
{
	void movement(Window& window, entt::registry& registry, const nlohmann::json& constants)
	{
		auto view = registry.view<Comps::Movement, Comps::Position>();

		for (const entt::entity entity : view)
		{
			auto [movement, position] = view.get<Comps::Movement, Comps::Position>(entity);
			position.pos += movement.vel;
		}
	}
}