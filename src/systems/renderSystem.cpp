#include "renderSystem.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/multitexture.h"
#include "comps/texture.h"
#include "comps/position.h"
#include "comps/offset.h"
#include "comps/camera.h"

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

	const Vect<int32_t> getCameraOffset(entt::registry& registry)
	{
		// Finds camera entity and returns its offset
		const auto view = registry.view<Comps::Camera, Comps::Offset>();
		for (const entt::entity entity : view)
			return view.get<Comps::Offset>(entity).offset.cast<int32_t>();
		
		std::cout << "Unable to find camera entity" << std::endl;
		return Vect<int32_t>();
	}

	void renderTextures(entt::registry& registry, Window& window)
	{
		const Vect<int32_t> camOffset = getCameraOffset(registry);

		const auto view = registry.view<Comps::Texture, Comps::Position>();
		for (const entt::entity entity : view)
		{
			auto [texture, position] = view.get<Comps::Texture, Comps::Position>(entity);

			Vect<int32_t> pos = position.pos.cast<int32_t>();

			// If the entity follows the camera, add camera offset
			if (registry.all_of<Tags::FollowCamera>(entity))
				pos += camOffset;

			Systems::drawTex(texture, pos, window);
		}
	}

	void renderMultitextures(entt::registry& registry, Window& window)
	{
		const Vect<int32_t> camOffset = getCameraOffset(registry);

		const auto view = registry.view<Comps::MultiTexture, Comps::Position>();
		for (const entt::entity entity : view)
		{
			auto [multitexture, position] = view.get<Comps::MultiTexture, Comps::Position>(entity);

			// Iterate through all textures and render them at their offset from the position of the object
			for (const std::pair<Comps::Texture, Comps::Offset>& pair : multitexture.textures)
			{
				Vect<int32_t> pos = (position.pos + pair.second.offset).cast<int32_t>();
				
				if (registry.all_of<Tags::FollowCamera>(entity))
					pos += camOffset;

				Systems::drawTex(pair.first, pos, window);
			}
		}
	}
}