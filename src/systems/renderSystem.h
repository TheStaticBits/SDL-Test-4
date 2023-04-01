#pragma once

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/texture.h"
#include "utility/vect.h"

namespace Systems
{
	// Renders objects with Comps::Texture and Comps::Multitexture
	void render(entt::registry& registry, Window& window);

	// Renders a single texture object at a given position
	void drawTex(const Comps::Texture& texture, const Vect<int32_t>& position, Window& window);

	// Retrieves camera offset
	Vect<int32_t>& getCameraOffset(entt::registry& registry);

	void renderTextures(entt::registry& registry, Window& window);
	void renderMultitextures(entt::registry& registry, Window& window);
}