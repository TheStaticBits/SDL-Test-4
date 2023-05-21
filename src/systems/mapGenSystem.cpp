#include "systems/mapGenSystem.h"

#include <entt/entt.hpp>

#include "comps/camera.h"
#include "comps/offset.h"
#include "helpers/mapGenHelpers.h"
#include "utility/enttUtility.h"

namespace Systems
{
	void updateMapGeneration(entt::registry& registry, Window& window, const nlohmann::json& constants)
	{
		const float cameraYOffset = Utility::getEnttComp<Comps::Offset, Comps::Camera>(registry)->offset.y;
		Comps::LayerGen* layerGen = Helpers::getLayerGen(registry);

		// Generate until the window is filled with layers
		while (layerGen->yPos - cameraYOffset < layerGen->windowHeight)
		{
			Helpers::genLayer(registry, window, constants);
			Helpers::startNextLayerGen(registry);
		}
	}
}
