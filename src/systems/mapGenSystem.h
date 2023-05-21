#pragma once

#include <entt/entt.hpp>

#include "helpers/mapGenHelpers.h"

namespace Systems
{
	void updateMapGeneration(entt::registry& registry, Window& window, const nlohmann::json& constants);
}