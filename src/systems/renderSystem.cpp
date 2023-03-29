#include "renderSystem.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/multitexture.h"
#include "comps/texture.h"
#include "comps/position.h"

namespace Systems
{
	void render(entt::registry& registry, Window& window)
	{
		renderTextures(registry, window);
		renderMultitextures(registry, window);
	}

	void drawTex(const Comps::Texture& texture, const Vect<int32_t>& position, Window& window)
	{
		window.render(texture.tex, position.toRect(texture.destSize.cast<int32_t>()));
	}

	void renderTextures(entt::registry& registry, Window& window)
	{
		auto view = registry.view<Comps::Texture, Comps::Position>();

		for (const entt::entity entity : view)
		{
			auto [texture, position] = view.get<Comps::Texture, Comps::Position>(entity);
			Systems::drawTex(texture, position.pos.cast<int32_t>(), window);
		}
	}

	void renderMultitextures(entt::registry& registry, Window& window)
	{
		auto view = registry.view<Comps::MultiTexture, Comps::Position>();

		for (const entt::entity entity : view)
		{
			auto [multitexture, position] = view.get<Comps::MultiTexture, Comps::Position>(entity);

			// Iterate through all textures and render them at their offset from the position of the object
			for (const std::pair<Comps::Texture, Vect<int32_t>>& pair : multitexture.textures)
				Systems::drawTex(pair.first, position.pos.cast<int32_t>() + pair.second, window);
		}
	}
}