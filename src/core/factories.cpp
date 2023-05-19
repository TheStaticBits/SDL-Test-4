#include "core/factories.h"

#include <iostream>
#include <vector>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/texture.h"
#include "core/save.h"

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
#include "comps/hitbox.h"

#include "utility/enttUtility.h"

namespace Factories
{
	entt::entity makePlayer(entt::registry& registry, Window& window, const nlohmann::json& constants, Save& save)
	{
		entt::entity entity = registry.create();

		const uint32_t scale = Helpers::getTextureScale(constants);

		// Comps
		registry.emplace<Comps::Texture>(entity, Helpers::makeTexture(constants["player"]["image"].get<std::string>(), window, constants));

		registry.emplace<Comps::Movement>(entity, Vect<float>(), Vect<float>());
		registry.emplace<Comps::KeyboardInput>(entity, Vect<float>(constants["player"]["acceleration"]));
		registry.emplace<Comps::MaxSpeed>(entity, Vect<float>(constants["player"]["maxSpeed"]));
		registry.emplace<Comps::Gravity>(entity, constants["player"]["gravity"].get<float>());
		registry.emplace<Comps::Player>(entity, constants["player"]["bounceVel"].get<float>());

		// Hitbox
		auto& hitbox = registry.emplace<Comps::Hitbox>(entity);
		const Vect<int32_t> hitboxOffset(constants["player"]["hitbox"]["offset"]);
		const Vect<uint32_t> hitboxSize(constants["player"]["hitbox"]["size"]);
		hitbox.boxes.push_back(Comps::Box{ hitboxOffset.cast<float>() * static_cast<float>(scale), hitboxSize * scale });

		if (save.isEmpty() || save.shouldIgnore())
			registry.emplace<Comps::Position>(entity, Vect<float>(constants["player"]["startingPos"])); // Default
		else
			registry.emplace<Comps::Position>(entity, Vect<float>(save.data()["player"]["position"])); // Read from save


		Comps::Collision& col = registry.emplace<Comps::Collision>(entity);

		// Callback for when player collides with tile on y axis
		col.callbacks.y = [](entt::registry& registry, entt::entity entity, entt::entity collidedEntity) {
			float& velocity = registry.get<Comps::Movement>(entity).vel.y;

			if (velocity > 0) // Touched tile while moving down
			{
				registry.destroy(collidedEntity);
				velocity = registry.get<Comps::Player>(entity).bounceVelocity;
			}
			else velocity = 0; // Touched tile while moving up
		};

		// X axis collision callback
		col.callbacks.x = [](entt::registry& registry, entt::entity entity, entt::entity collidedEntity) {
			registry.get<Comps::Movement>(entity).vel.x = 0;
		};


		// Tags
		registry.emplace<Tags::CameraFollow>(entity);

		return entity;
	}

	entt::entity makeCamera(entt::registry& registry, const nlohmann::json& constants)
	{
		entt::entity entity = registry.create();
		
		registry.emplace<Comps::Offset>(entity, Vect<float>());
		registry.emplace<Comps::Camera>(entity, constants["camera"]["followSpeed"].get<float>(), 
												Vect<float>(constants["camera"]["offset"]), 
												Vect<bool>(constants["camera"]["lockedDirs"]));
		
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


		auto& multiTex = registry.emplace<Comps::MultiTexture>(entity);
		// Generating individual tile texture parts for the Multitexture object
		for (const auto& imageData : constants["tiles"]["layouts"][layoutType]["images"]) // Images of the tile
		{
			Comps::TexturePart texPart;

			texPart.texture = tileTextures->textures.at(tileType); // Copy texture of the tile type

			texPart.texture.srcSize = Vect<uint32_t>(imageData["srcRect"][1]);
			texPart.texture.offset =  Vect<uint32_t>(imageData["srcRect"][0]); // src rect offset
			texPart.texture.destSize = texPart.texture.srcSize * scale; // draw size

			// offset from the position of the object
			texPart.offset = Comps::Offset{ Vect<float>(imageData["offset"]) * static_cast<float>(scale) }; 
			
			multiTex.textures.push_back(texPart); // Adding the texture and offset to the vector
		}


		// Generating hitboxes for Comps::Hitboxes from the constants JSON
		auto& hitbox = registry.emplace<Comps::Hitbox>(entity);
		for (const auto& hitboxData : constants["tiles"]["layouts"][layoutType]["hitbox"])
		{
			Comps::Box box;
			box.offset = Comps::Offset{ Vect<float>(hitboxData["offset"]) * static_cast<float>(scale)}; // hitbox position
			box.size = Vect<uint32_t>(hitboxData["size"]) * scale;

			hitbox.boxes.push_back(box);
		}

		
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
		
		auto& layerGen = registry.emplace<Comps::LayerGen>(entity);
		layerGen.yPos = constants["world"]["startY"].get<uint32_t>();

		registry.emplace<Tags::LayerGen>(entity);

		return entity;
	}

	const std::string randomTile(const nlohmann::json& constants)
	{
		uint32_t randomNum = rand() % 100 + 1; // 1 - 100
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