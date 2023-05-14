#include "mapGenHelpers.h"

#include <nlohmann/json.hpp>

#include "comps/layerGen.h"
#include "comps/texStorage.h"
#include "core/factories.h"

#include "helpers/textureHelpers.h"

#include "utility/enttUtility.h"

namespace Helpers
{
	void setupLayerGen(entt::registry& registry, const nlohmann::json& constants)
	{
		uint32_t mapWidth = constants["display"]["size"][0].get<uint32_t>() / constants["tiles"]["size"].get<int32_t>();

		// Resizing map generation layer vectors
		Comps::LayerGen* layerGen = Helpers::getLayerGen(registry);
		layerGen->layers.resize(getTallestTileHeight(constants)); // Resize height of the 2D array

		for (std::vector<bool>& layer : layerGen->layers)
			layer.resize(mapWidth, false); // Setting all layers to false by default and the width of the map
	}

	void startNextLayerGen(entt::registry& registry)
	{
		Comps::LayerGen* layerGen = Helpers::getLayerGen(registry);

		int32_t mapWidth = layerGen->layers[0].size();

		// Removes the first layer (as it has finished generating)
		layerGen->layers.erase(layerGen->layers.begin());

		// Add new layer with same width and all initially false
		layerGen->layers.push_back(std::vector<bool>());
		layerGen->layers[layerGen->layers.size() - 1].resize(mapWidth, false);
	}

	void genLayer(entt::registry& registry, Window& window, const nlohmann::json& constants)
	{
		Comps::LayerGen* layerGen = Helpers::getLayerGen(registry);

		const uint32_t tileSize = constants["tiles"]["size"].get<uint32_t>() * getTextureScale(constants);

		for (uint32_t xCoord = 0; xCoord < layerGen->layers[0].size(); xCoord++)
			if (!layerGen->layers[0][xCoord]) // If space not taken up by a tile yet
				Factories::makeTile(registry, window, constants, xCoord, Vect<uint32_t>(xCoord * tileSize, layerGen->yPos));

		// Increment the layer's y position
		layerGen->yPos += tileSize;
	}

	void fillSpots(entt::registry& registry, const uint32_t xCoord, const nlohmann::json& offsets)
	{
		Comps::LayerGen* layerGen = Helpers::getLayerGen(registry);

		// Iterating through all offsets from (xCoord, 0)
		// (generating tiles are always going to be in the first array of layerGen->layers, so y is always 0)
		// and setting spaces that were taken up to true
		for (const auto& coordOffset : offsets)
		{
			Vect<uint32_t> offset(coordOffset);
			offset.x += xCoord;

			if (offset.x >= layerGen->layers[0].size())
				continue; // If offset is out of bounds, skip

			layerGen->layers[offset.y][offset.x] = true;
		}
	}

	const bool spotsAvailable(entt::registry& registry, const uint32_t xCoord, const nlohmann::json& offsets)
	{
		Comps::LayerGen* layerGen = Helpers::getLayerGen(registry);

		for (const auto& coordOffset : offsets)
		{
			Vect<uint32_t> offset(coordOffset);
			offset.x += xCoord;

			if (offset.x >= layerGen->layers[0].size())
				continue;

			if (layerGen->layers[offset.y][offset.x])
				return false;
		}

		return true;
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

	Comps::LayerGen* getLayerGen(entt::registry& registry)
	{
		return Utility::getEnttComp<Comps::LayerGen, Tags::LayerGen>(registry);
	}
}