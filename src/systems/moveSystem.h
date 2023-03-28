#pragma once

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"

namespace Systems
{
	void movement(Window& window, entt::registry& registry, const nlohmann::json& constants);
}