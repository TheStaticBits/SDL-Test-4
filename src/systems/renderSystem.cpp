#include "renderSystem.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/multitexture.h"
#include "comps/texture.h"
#include "comps/position.h"
#include "comps/offset.h"
#include "comps/camera.h"

#include "utility/enttUtility.h"

namespace Systems
{
	void render(entt::registry& registry, Window& window)
	{
		renderTextures(registry, window);
		renderMultitextures(registry, window);
	}

	void drawTex(const Comps::Texture& texture, const Vect<uint32_t>& position, Window& window)
	{
		window.render(texture.tex, Vect<uint32_t>::toRect(texture.offset, texture.srcSize),
								   Vect<uint32_t>::toRect(position, texture.destSize));
	}

	void renderEntity(entt::registry& registry, Window& window, const entt::entity entity, const Comps::Texture& texture, Vect<uint32_t> pos)
	{
		// If the entity follows the camera, add camera offset
		if (registry.all_of<Tags::FollowCamera>(entity))
			pos -= Utility::getEnttComp<Comps::Offset, Comps::Camera>(registry).offset.cast<uint32_t>(); // Gets camera offset

		Systems::drawTex(texture, pos, window);
	}

	void renderTextures(entt::registry& registry, Window& window)
	{
		const auto view = registry.view<Comps::Texture, Comps::Position>();
		for (const entt::entity entity : view)
		{
			auto [texture, position] = view.get<Comps::Texture, Comps::Position>(entity);
			Systems::renderEntity(registry, window, entity, texture, position.pos.cast<uint32_t>());
		}
	}

	void renderMultitextures(entt::registry& registry, Window& window)
	{
		const auto view = registry.view<Comps::MultiTexture, Comps::Position>();
		for (const entt::entity entity : view)
		{
			auto [multitexture, position] = view.get<Comps::MultiTexture, Comps::Position>(entity);

			// Iterate through all textures and render them at their offset from the position of the object
			for (const std::pair<Comps::Texture, Comps::Offset>& texPair : multitexture.textures)
			{
				Vect<uint32_t> pos = (position.pos + texPair.second.offset).cast<uint32_t>(); // applying texture offset to base position
				Systems::renderEntity(registry, window, entity, texPair.first, pos);
			}
		}
	}
}