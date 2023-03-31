#include "helpers/textureHelpers.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "utility/vect.h"
#include "core/window.h"
#include "comps/texture.h"
#include "comps/multitexture.h"

namespace Helpers
{
	[[nodiscard]] Comps::Texture makeTexture(std::string path, Window& window, const nlohmann::json& constants)
	{
		// Loading texture
		SDL_Texture* texture = window.loadTexture(path);

		// Getting scale from constants
		uint32_t scale = constants["display"]["scale"].get<uint32_t>();

		// Get size of texture
		int x, y;
		SDL_QueryTexture(texture, NULL, NULL, &x, &y);
		Vect<uint32_t> size(x, y);
		Vect<uint32_t> destSize = size * scale;

		// Creating texture object
		Comps::Texture texObj{ texture, size, destSize };
		return texObj;
	}


	void destroyTextures(entt::registry& registry)
	{
		// Destroy all texture component's SDL texture
		auto view = registry.view<Comps::Texture>();
		for (const entt::entity entity : view)
			SDL_DestroyTexture(view.get<Comps::Texture>(entity).tex);

		// Destroy all textures in each multitexture component
		auto viewMultitextures = registry.view<Comps::MultiTexture>();
		for (const entt::entity entity : viewMultitextures)
			for (const std::pair<Comps::Texture, Vect<int32_t>>& pair : viewMultitextures.get<Comps::MultiTexture>(entity).textures)
				SDL_DestroyTexture(pair.first.tex);
	}
}
