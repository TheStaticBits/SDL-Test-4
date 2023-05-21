#include "playerHelpers.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "comps/movement.h"
#include "comps/player.h"
#include "comps/position.h"
#include "comps/texture.h"

#include "core/save.h"

namespace Helpers
{
	void playerSave(entt::registry& registry, Save& save)
	{
		const auto view = registry.view<Comps::Position, Comps::Player>();
		for (const auto entity : view)
		{
			auto& position = view.get<Comps::Position>(entity);
			save.data()["player"]["position"][0] = position.pos.x;
			save.data()["player"]["position"][1] = position.pos.y;
		}
	}

	void updatePlayerBoundaries(entt::registry& registry)
	{
		const auto view = registry.view<Comps::Movement, Comps::Position, Comps::Player, Comps::Texture>();
		for (const auto entity : view)
		{
			auto [mov, pos, player, tex] = view.get<Comps::Movement, Comps::Position, Comps::Player, Comps::Texture>(entity);

			if (pos.pos.x < player.boundaries.x) // Left boundary
			{
				pos.pos.x = player.boundaries.x;
				mov.vel.x = 0;
			}
			else if (pos.pos.x + tex.destSize.x > player.boundaries.y) // Right boundary
			{
				pos.pos.x = player.boundaries.y - tex.destSize.x;
				mov.vel.x = 0;
			}
		}
	}
}