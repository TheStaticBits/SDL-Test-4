#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

#include "comps/layerGen.h"
#include "core/window.h"

#include "utility/vect.h"

namespace Helpers
{
	void setupLayerGen(entt::registry& registry, const nlohmann::json& constants);
	void startNextLayerGen(entt::registry& registry);

	void genLayer(entt::registry& registry, Window& window, const nlohmann::json& constants);
	void fillSpots(entt::registry& registry, const uint32_t xCoord, const nlohmann::json& offsets);
	const bool spotsAvailable(entt::registry& registry, const uint32_t xCoord, const nlohmann::json& offsets);

	const uint32_t getTallestTileHeight(const nlohmann::json& constants);

	inline Comps::LayerGen* getLayerGen(entt::registry& registry);
}