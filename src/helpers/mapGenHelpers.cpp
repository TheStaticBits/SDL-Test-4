#include "mapGenHelpers.h"

#include <nlohmann/json.hpp>

#include "comps/layerGen.h"
#include "comps/texStorage.h"

namespace Helpers
{
	void setupLayerGen(entt::registry& registry, const nlohmann::json& constants)
	{
		int32_t mapWidth = constants["display"]["size"][0].get<int32_t>();

		const auto viewTexStorage = registry.view<Comps::TextureStorage>();
		for (const entt::entity entity : viewTexStorage)
		{
			auto& texStorage = viewTexStorage.get<Comps::TextureStorage>(entity);

			// Find texture width, divide window width by texture width to get the amount of tiles per layer
			int32_t texWidth = (texStorage.textures.begin()->second).destSize.xCast<int32_t>();
			mapWidth /= texWidth;
		}

		const auto viewLayerGen = registry.view<Comps::LayerGen>();
		for (const entt::entity entity : viewLayerGen)
		{
			auto& layerGen = viewLayerGen.get<Comps::LayerGen>(entity);

			// Resize the layer vectors to the amount of tiles per layer, setting all elements to false
			layerGen.prevLayer.resize(mapWidth, false);
			layerGen.nextLayer.resize(mapWidth, false);
		}
	}

	void startNextLayerGen(entt::registry& registry)
	{
		const auto view = registry.view<Comps::LayerGen>();
		for (const entt::entity entity : view)
		{
			auto& layerGen = view.get<Comps::LayerGen>(entity);

			// Move down a layer
			layerGen.prevLayer = layerGen.nextLayer;
			
			// Set next layer values to false to prepare for the generation of this layer
			std::fill(layerGen.nextLayer.begin(), layerGen.nextLayer.end(), false);
		}
	}
}