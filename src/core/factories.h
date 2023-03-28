#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"

namespace Factories
{
	entt::entity makePlayer(entt::registry& registry, Window& window, const nlohmann::json& constants);
};