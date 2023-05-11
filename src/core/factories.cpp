#include "core/factories.h"

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/texture.h"

#include "helpers/textureHelpers.h"
#include "comps/position.h"
#include "comps/movement.h"
#include "comps/player.h"
#include "comps/layerGen.h"
#include "comps/texStorage.h"
#include "comps/offset.h"
#include "comps/camera.h"

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
			registry.emplace<Comps::Position>(entity, Vect<float>());

		// Tags
		registry.emplace<Tags::KeyboardInput>(entity);
		registry.emplace<Tags::FollowCamera>(entity);
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

	entt::entity makeTile(entt::registry& registry, Window& window, const nlohmann::json& constants, const Vect<int32_t> pos)
	{
		entt::entity entity = registry.create();

		// todo

		return entity;
	}

	entt::entity makeTileTextureStorage(entt::registry& registry, Window& window, const nlohmann::json& constants)
	{
		entt::entity entity = registry.create();
		registry.emplace<Comps::TextureStorage>(entity);
		return entity;
	}

	entt::entity makeLayerGen(entt::registry& registry, const nlohmann::json& constants)
	{
		entt::entity entity = registry.create();
		registry.emplace<Comps::LayerGen>(entity, std::vector<bool>(), std::vector<bool>(), constants["game"]["startYPos"].get<uint32_t>());
		return entity;
	}
}