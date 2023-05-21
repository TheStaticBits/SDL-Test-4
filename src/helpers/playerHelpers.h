#pragma once

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/save.h"

namespace Helpers
{
	void playerSave(entt::registry& registry, Save& save);

	void updatePlayerBoundaries(entt::registry& registry);
}