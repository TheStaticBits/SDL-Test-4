#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"

namespace Factories
{
	entt::entity makePlayer(entt::registry& registry, Window& window, const nlohmann::json& constants, const nlohmann::json& save);
	entt::entity makeCamera(entt::registry& registry);

	entt::entity makeTile(entt::registry& registry, Window& window, const nlohmann::json& constants);
	entt::entity makeTileTextureStorage(entt::registry& registry, Window& window, const nlohmann::json& constants);
	entt::entity makeLayerGen(entt::registry& registry, const nlohmann::json& constants);
}