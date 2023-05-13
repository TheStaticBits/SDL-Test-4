#include "mapGenHelpers.h"

#include <nlohmann/json.hpp>

#include "comps/layerGen.h"
#include "comps/texStorage.h"
#include "core/factories.h"

namespace Helpers
{
	void setupLayerGen(entt::registry& registry, const nlohmann::json& constants)
	{
		int32_t mapWidth = constants["display"]["size"][0].get<int32_t>();

		// Finding map width based on tile texture sizes
		const auto viewTexStorage = registry.view<Comps::TextureStorage>();
		for (const entt::entity entity : viewTexStorage)
		{
			auto& texStorage = viewTexStorage.get<Comps::TextureStorage>(entity);

			// Find texture width, divide window width by texture width to get the amount of tiles per layer
			int32_t texWidth = (texStorage.textures.begin()->second).destSize.xCast<int32_t>();
			mapWidth /= texWidth;
			break;
		}

		// Resizing map generation layer vectors
		const auto viewLayerGen = registry.view<Comps::LayerGen>();
		for (const entt::entity entity : viewLayerGen)
		{
			auto& layerGen = viewLayerGen.get<Comps::LayerGen>(entity);

			layerGen.layers.resize(getTallestTileHeight(constants)); // Resize height of the 2D array

			for (std::vector<bool>& layer : layerGen.layers)
				layer.resize(mapWidth, false); // Setting all layers to false by default and the width of the map
		}
	}

	void startNextLayerGen(entt::registry& registry)
	{
		const auto view = registry.view<Comps::LayerGen>();
		for (const entt::entity entity : view)
		{
			auto& layerGen = view.get<Comps::LayerGen>(entity);
			int32_t mapWidth = layerGen.layers[0].size();

			// Removes the first layer (as it has finished generating)
			layerGen.layers.erase(layerGen.layers.begin());

			// Add new layer with same width and all initially false
			layerGen.layers.push_back(std::vector<bool>());
			layerGen.layers[layerGen.layers.size() - 1].resize(mapWidth, false);
		}
	}

	void genLayer(entt::registry& registry, const nlohmann::json& constants)
	{
		// todo
	}

	void fillSpots(entt::registry& registry, const Vect<uint32_t>& coords, const nlohmann::json& offsets)
	{
		// todo
	}

	const uint32_t getTallestTileHeight(const nlohmann::json& constants)
	{
		uint32_t height = 1;

		// Finding the tallest tile size
		for (const auto& layouts : constants["tiles"]["layouts"].items())
		{
			for (const auto& tilePositions : layouts.value()["takesUpTiles"])
			{
				uint32_t tileHeight = tilePositions[1].get<uint32_t>() + 1;
				if (tileHeight > height)
					height = tileHeight;
			}
		}

		return height;
	}
}