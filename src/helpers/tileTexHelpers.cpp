#include "tileTexHelpers.h"

#include <vector>

#include "core/window.h"

#include "comps/texStorage.h"
#include "helpers/textureHelpers.h"

namespace Helpers
{
	void generateTileTextures(entt::registry& registry, Window& window, const nlohmann::json& constants)
	{
		std::string tileImgPath = constants["tiles"]["image"].get<std::string>();

		const auto view = registry.view<Comps::TextureStorage>();
		for (const entt::entity entity : view)
		{
			auto& texStorage = view.get<Comps::TextureStorage>(entity);
			const nlohmann::json tileTypes = constants["tiles"]["types"].items();
			for (auto i = tileTypes.begin(); i != tileTypes.end(); i++) // Iterate through all tile types
			{
				Comps::Texture tex = Helpers::makeTexture(tileImgPath, window, constants); // Load texture

				Helpers::modColor(tex, i.value()["color"].get<std::vector<uint8_t>>()); // modifies color by srcC = srcC * (color / 255)
				
				texStorage.textures[i.key()] = tex; // Add to texture storage
			}
		}
	}
}