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
		registry.emplace<Comps::Texture>(entity, Helpers::makeTexture("", window, constants));
		registry.emplace<Comps::Position>(entity, Vect<int32_t>(0, 0));
		registry.emplace<Comps::Movement>(entity, Vect<int32_t>(0, 0));

		return entity;
	}
};