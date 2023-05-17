#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"
#include "core/save.h"

namespace Factories
{
	entt::entity makePlayer(entt::registry& registry, Window& window, const nlohmann::json& constants, Save& save);
	entt::entity makeCamera(entt::registry& registry, const nlohmann::json& constants);

	entt::entity makeTile(entt::registry& registry, Window& window, const nlohmann::json& constants, const uint32_t xCoord, const Vect<uint32_t> pos);
	entt::entity makeTileTextureStorage(entt::registry& registry, Window& window, const nlohmann::json& constants);
	entt::entity makeLayerGen(entt::registry& registry, const nlohmann::json& constants);


	const std::string randomTile(const nlohmann::json& constants);
}