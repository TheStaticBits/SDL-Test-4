#include "playerHelpers.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "comps/player.h"
#include "comps/position.h"

namespace Helpers
{
	void playerSave(entt::registry& registry, nlohmann::json& save)
	{
		const auto view = registry.view<Comps::Position, Tags::Player>();
		for (const auto entity : view)
		{
			auto& position = view.get<Comps::Position>(entity);
			save["player"]["position"][0] = position.pos.x;
			save["player"]["position"][1] = position.pos.y;
		}
	}
}