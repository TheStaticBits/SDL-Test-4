#include "systems/cameraSystem.h"

#include "comps/position.h"
#include "comps/player.h"
#include "comps/camera.h"
#include "comps/offset.h"
#include "comps/texture.h"

#include "systems/moveSystem.h"
#include "utility/vect.h"
#include "utility/enttUtility.h"

namespace Systems
{
	void updateCameraMovement(entt::registry& registry)
	{
		const Vect<float> followObjPos = Utility::getEnttComp<Comps::Position, Tags::CameraFollow>(registry).pos;
		const Vect<uint32_t> followObjSize = Utility::getEnttComp<Comps::Texture, Tags::CameraFollow>(registry).destSize;

		const auto view = registry.view<Comps::Camera, Comps::Offset>();
		for (const entt::entity entity : view)
		{
			auto [camera, offset] = view.get<Comps::Camera, Comps::Offset>(entity);
			const Vect<float> moveTo = followObjPos + (followObjSize.cast<float>() / 2); // Centered on the object to follow

			offset.offset += ((moveTo + camera.offset) - offset.offset) / camera.followSpeed;

			if (camera.lockedDirs.x) { offset.offset.x = 0; }
			if (camera.lockedDirs.y) { offset.offset.y = 0; }
		}
	}
}