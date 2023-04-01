#include "systems/cameraSystem.h"

#include "comps/position.h"
#include "comps/player.h"

#include "systems/moveSystem.h"

namespace Systems
{
	void Systems::updateCameraMovement(entt::registry& registry)
	{
		// todo
	}

	const Vect<int32_t>& getPlayerPos(entt::registry& registry)
	{
		const auto view = registry.view<Comps::Position, Tags::Player>();
		for (const entt::entity entity : view)
			return view.get<Comps::Position>(entity).pos.cast<int32_t>();
	}
}