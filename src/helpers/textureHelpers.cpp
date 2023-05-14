#include "helpers/textureHelpers.h"

#include <iostream>
#include <vector>

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

		// Get size of texture
		int width, height;
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
		Vect<uint32_t> size(width, height);

		// Creating texture object
		return Comps::Texture{ texture, Vect<uint32_t>(0, 0), size, size * Helpers::getTextureScale(constants) };
	}

	void destroyTextures(entt::registry& registry)
	{
		// Destroy all texture component's SDL texture
		const auto viewTexs = registry.view<Comps::Texture>();
		for (const entt::entity entity : viewTexs)
			SDL_DestroyTexture(viewTexs.get<Comps::Texture>(entity).tex);

		// Destroy all textures in each multitexture component
		const auto viewMult = registry.view<Comps::MultiTexture>();
		for (const entt::entity entity : viewMult)
			for (const std::pair<Comps::Texture, Comps::Offset>& pair : viewMult.get<Comps::MultiTexture>(entity).textures)
				SDL_DestroyTexture(pair.first.tex);
	}

	void modColor(Comps::Texture& texture, const std::vector<uint8_t>& color)
	{
		// Modifying color of texture
		SDL_SetTextureColorMod(texture.tex, color[0], color[1], color[2]);
	}

	const uint32_t getTextureScale(const nlohmann::json& constants)
	{
		// Getting scale from constants
		static uint32_t scale = constants["display"]["scale"].get<uint32_t>();
		return scale;
	}
}
