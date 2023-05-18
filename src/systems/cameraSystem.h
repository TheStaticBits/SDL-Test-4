#pragma once

#include <entt/entt.hpp>

#include "core/window.h"

namespace Systems
{
	void updateCameraMovement(entt::registry& registry, Window& window);
}