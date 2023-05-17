#include "playerHelpers.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "comps/player.h"
#include "comps/position.h"

#include "core/save.h"

namespace Helpers
{
	void playerSave(entt::registry& registry, Save& save)
	{
		const auto view = registry.view<Comps::Position, Tags::Player>();
		for (const auto entity : view)
		{
			auto& position = view.get<Comps::Position>(entity);
			save.data()["player"]["position"][0] = position.pos.x;
			save.data()["player"]["position"][1] = position.pos.y;
		}
	}
}