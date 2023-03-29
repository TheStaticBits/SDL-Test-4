#include "core/factories.h"

#include <iostream>
#include "core/factories.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "core/window.h"
#include "comps/texture.h"

#include "helpers/textureHelpers.h"
#include "comps/position.h"
#include "comps/movement.h"
#include "comps/player.h"

namespace Factories
{
	entt::entity makePlayer(entt::registry& registry, Window& window, const nlohmann::json& constants, const nlohmann::json& save)
	{
		entt::entity entity = registry.create();

		// Comps
		registry.emplace<Comps::Texture>(entity, Helpers::makeTexture(constants["player"]["image"].get<std::string>(), window, constants));
		registry.emplace<Comps::Movement>(entity, Vect<float>(0.0f, 0.0f), 
										  constants["player"]["acceleration"].get<float>(), 
										  constants["player"]["maxSpeed"].get<float>());

		if (!save.empty())
			registry.emplace<Comps::Position>(entity, Vect<float>(save["player"]["position"]));
		else
			registry.emplace<Comps::Position>(entity, Vect<float>(0.0f, 0.0f));

		// Tags
		registry.emplace<Tags::KeyboardInput>(entity);
		registry.emplace<Tags::Player>(entity);

		return entity;
	}
};