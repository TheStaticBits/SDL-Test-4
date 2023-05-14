#include "core/factories.h"

#include <iostream>
#include <vector>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/texture.h"

#include "helpers/textureHelpers.h"
#include "helpers/mapGenHelpers.h"

#include "comps/position.h"
#include "comps/movement.h"
#include "comps/player.h"
#include "comps/layerGen.h"
#include "comps/texStorage.h"
#include "comps/offset.h"
#include "comps/camera.h"
#include "comps/multitexture.h"

#include "utility/enttUtility.h"

namespace Factories
{
	entt::entity makePlayer(entt::registry& registry, Window& window, const nlohmann::json& constants, const nlohmann::json& save)
	{
		entt::entity entity = registry.create();

		// Comps
		registry.emplace<Comps::Texture>(entity, Helpers::makeTexture(constants["player"]["image"].get<std::string>(), window, constants));

		registry.emplace<Comps::Movement>(entity, Vect<float>());
		registry.emplace<Comps::Acceleration>(entity, Vect<float>());
		registry.emplace<Comps::StaticAcceleration>(entity, Vect<float>(constants["player"]["acceleration"].get<float>(), 0));
		registry.emplace<Comps::MaxSpeed>(entity, constants["player"]["maxSpeed"].get<float>());
		registry.emplace<Comps::Gravity>(entity, constants["player"]["gravity"].get<float>());

		if (!save.empty())
			registry.emplace<Comps::Position>(entity, Vect<float>(save["player"]["position"]));
		else
			registry.emplace<Comps::Position>(entity, Vect<float>(constants["player"]["startingPos"]));

		// Tags
		registry.emplace<Tags::KeyboardInput>(entity);
		registry.emplace<Tags::Player>(entity);
		registry.emplace<Tags::CameraFollow>(entity);

		return entity;
	}

	entt::entity makeCamera(entt::registry& registry, const nlohmann::json& constants)
	{
		entt::entity entity = registry.create();
		
		registry.emplace<Comps::Offset>(entity, Vect<float>());
		registry.emplace<Comps::Camera>(entity, constants["game"]["cameraFollowSpeed"].get<float>(), 
												Vect<float>(constants["game"]["cameraOffset"]), 
												Vect<bool>(constants["game"]["lockedDirs"]));
		
		return entity;
	}

	entt::entity makeTile(entt::registry& registry, Window& window, const nlohmann::json& constants, const uint32_t xCoord, const Vect<uint32_t> pos)
	{
		entt::entity entity = registry.create();

		registry.emplace<Comps::Position>(entity, pos.cast<float>());

		std::string tileType, layoutType;
		nlohmann::json tileHitboxCoords;

		// Generates a random tile continuously until a tile that does not overlap other tiles is found
		do {
			tileType = randomTile(constants);
			layoutType = constants["tiles"]["types"][tileType]["layout"].get<std::string>();
			tileHitboxCoords = constants["tiles"]["layouts"][layoutType]["takesUpTiles"];
		}  while (!Helpers::spotsAvailable(registry, xCoord, tileHitboxCoords));

		const Comps::TextureStorage* tileTextures = Utility::getEnttComp<Comps::TextureStorage, Tags::TileTextures>(registry);
		const uint32_t scale = Helpers::getTextureScale(constants);

		// Generating individual tile textures for the Multitexture object
		std::vector<std::pair<Comps::Texture, Comps::Offset>> texPairs;
		for (const auto& imageData : constants["tiles"]["layouts"][layoutType]["images"]) // Images of the tile
		{
			Comps::Texture copy = tileTextures->textures.at(tileType);

			copy.srcSize = Vect<uint32_t>(imageData["srcRect"][1]);
			copy.offset =  Vect<uint32_t>(imageData["srcRect"][0]); // src rect offset
			copy.destSize = copy.srcSize * scale; // draw size

			// offset from the position of the object
			Comps::Offset imageOffset { Vect<float>(imageData["offset"]) * static_cast<float>(scale) }; 
			
			texPairs.push_back(std::pair<Comps::Texture, Comps::Offset>(copy, imageOffset)); // Adding the texture and offset to the vector
		}

		registry.emplace<Comps::MultiTexture>(entity, texPairs);

		// Marks the spots that the tile takes up as taken
		Helpers::fillSpots(registry, xCoord, constants["tiles"]["layouts"][layoutType]["takesUpTiles"]);

		return entity;
	}

	entt::entity makeTileTextureStorage(entt::registry& registry, Window& window, const nlohmann::json& constants)
	{
		entt::entity entity = registry.create();
		
		registry.emplace<Comps::TextureStorage>(entity);
		registry.emplace<Tags::TileTextures>(entity);

		return entity;
	}

	entt::entity makeLayerGen(entt::registry& registry, const nlohmann::json& constants)
	{
		entt::entity entity = registry.create();
		
		registry.emplace<Comps::LayerGen>(entity, std::vector<std::vector<bool>>(), constants["world"]["startY"].get<uint32_t>());
		registry.emplace<Tags::LayerGen>(entity);

		return entity;
	}

	const std::string randomTile(const nlohmann::json& constants)
	{
		uint32_t randomNum = rand() % 100 + 1;
		std::string tileType = "";

		// Choosing a tile
		for (const auto& typeData : constants["tiles"]["types"].items())
		{
			const uint32_t chance = typeData.value()["chance"].get<uint32_t>();

			if (randomNum <= chance)
			{
				tileType = typeData.key();
				break;
			}
			else randomNum -= chance;
		}

		return tileType;
	}
}