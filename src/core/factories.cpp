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

namespace Factories
{
	entt::entity makePlayer(entt::registry& registry, Window& window, const nlohmann::json& constants)
	{
		entt::entity entity = registry.create();

		registry.emplace<Comps::Texture>(entity, Helpers::makeTexture(constants["player"]["image"].get<std::string>(), window, constants));
		registry.emplace<Comps::Position>(entity, Vect<float>(0.0f, 0.0f));
		registry.emplace<Comps::Movement>(entity, Vect<float>(0.0f, 0.0f), 
										  constants["player"]["acceleration"].get<float>(), 
										  constants["player"]["maxSpeed"].get<float>());
		registry.emplace<Tags::KeyboardInput>(entity);

		return entity;
	}
};