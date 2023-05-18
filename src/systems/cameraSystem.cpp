#include "systems/cameraSystem.h"

#include "core/window.h"

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
	void updateCameraMovement(entt::registry& registry, Window& window)
	{
		const Vect<float> followObjPos = Utility::getEnttComp<Comps::Position, Tags::CameraFollow>(registry)->pos;
		const Vect<uint32_t> followObjSize = Utility::getEnttComp<Comps::Texture, Tags::CameraFollow>(registry)->destSize;
		const Vect<float> objCenter = followObjPos + (followObjSize.cast<float>() / 2); // Centered on the object to follow

		const auto view = registry.view<Comps::Camera, Comps::Offset>();
		for (const entt::entity entity : view)
		{
			auto [camera, offset] = view.get<Comps::Camera, Comps::Offset>(entity);

			const Vect<float> moveTo = objCenter + camera.offset; // Where the camera should move to
			const Vect<float> travelDist = (moveTo - offset.offset) * camera.followSpeed * window.getDeltaTime(); // this frame's movement

			offset.offset += travelDist; // Move the camera

			if (camera.lockedDirs.x) { offset.offset.x = 0; }
			if (camera.lockedDirs.y) { offset.offset.y = 0; }

			//if (travelDist.x)
		}
	}
}