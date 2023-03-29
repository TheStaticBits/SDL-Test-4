#pragma once

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

namespace Helpers
{
	void playerSave(entt::registry& registry, nlohmann::json& save);
}