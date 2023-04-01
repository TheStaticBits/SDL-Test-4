#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "utility/vect.h"
#include "core/window.h"
#include "comps/texture.h"

namespace Helpers
{
	[[nodiscard]] Comps::Texture makeTexture(std::string path, Window& window, const nlohmann::json& constants);

	void destroyTextures(entt::registry& registry);

	void modColor(Comps::Texture& texture, const std::vector<uint8_t>& color);
}