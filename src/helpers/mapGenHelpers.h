#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

#include "utility/vect.h"

namespace Helpers
{
	void setupLayerGen(entt::registry& registry, const nlohmann::json& constants);
	void startNextLayerGen(entt::registry& registry);

	void genLayer(entt::registry& registry, const nlohmann::json& constants);
	void fillSpots(entt::registry& registry, const Vect<uint32_t>& coords, const nlohmann::json& offsets);

	const uint32_t getTallestTileHeight(const nlohmann::json& constants);
}