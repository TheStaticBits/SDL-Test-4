#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

namespace Helpers
{
	void setupLayerGen(entt::registry& registry, const nlohmann::json& constants);

	void startNextLayerGen(entt::registry& registry);
}