#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

#include "core/window.h"

namespace Helpers
{
	void generateTileTextures(entt::registry& registry, Window& window, const nlohmann::json& constants);
}