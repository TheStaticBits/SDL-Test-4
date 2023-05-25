#include "renderSystem.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/multitexture.h"
#include "comps/texture.h"
#include "comps/position.h"
#include "comps/offset.h"
#include "comps/camera.h"
#include "comps/player.h"

#include "utility/enttUtility.h"
#include "utility/collision.h"

namespace Systems
{
	void render(entt::registry& registry, Window& window)
	{
		renderTextures(registry, window);
		renderMultitextures(registry, window);
	}

	void drawTex(const Comps::Texture& texture, const Vect<int32_t>& position, Window& window)
	{
		// check if texture is onscreen
		if (!Utility::collision<int32_t>({0, 0},   window.getSize().cast<int32_t>(),
										position, texture.destSize.cast<int32_t>()))
			return;

		window.render(texture.tex, Vect<uint32_t>::toRect(texture.offset, texture.srcSize),
								   Vect<int32_t>::toRect(position, texture.destSize.cast<int32_t>()));
	}

	void renderEntity(entt::registry& registry, Window& window, const entt::entity entity, const Comps::Texture& texture, Vect<float> pos)
	{
		// If the entity follows the camera, add camera offset
		if (!registry.all_of<Tags::LockToFrame>(entity))
			pos -= Utility::getEnttComp<Comps::Offset, Comps::Camera>(registry)->offset; // Gets camera offset

		//if (registry.all_of<Comps::Player>(entity))
		//	std::cout << pos.xCast<int32_t>() << std::endl;

		Systems::drawTex(texture, pos.cast<int32_t>(), window);
	}

	void renderTextures(entt::registry& registry, Window& window)
	{
		const auto view = registry.view<Comps::Texture, Comps::Position>();
		for (const entt::entity entity : view)
		{
			auto [texture, position] = view.get<Comps::Texture, Comps::Position>(entity);
			Systems::renderEntity(registry, window, entity, texture, position.pos);
		}
	}

	void renderMultitextures(entt::registry& registry, Window& window)
	{
		const auto view = registry.view<Comps::MultiTexture, Comps::Position>();
		for (const entt::entity entity : view)
		{
			auto [multitexture, position] = view.get<Comps::MultiTexture, Comps::Position>(entity);

			// Iterate through all textures and render them at their offset from the position of the object
			for (const Comps::TexturePart& texPart : multitexture.textures)
			{
				Vect<float> pos = position.pos + texPart.offset.offset; // applying texture offset to base position
				Systems::renderEntity(registry, window, entity, texPart.texture, pos);
			}
		}
	}
}