#include "systems/cameraSystem.h"

#include "comps/position.h"
#include "comps/player.h"
#include "comps/camera.h"
#include "comps/offset.h"
#include "comps/texture.h"

#include "systems/moveSystem.h"

namespace Systems
{
	void Systems::updateCameraMovement(entt::registry& registry)
	{
		// todo
		const Vect<float> playerPos = getPlayerPos(registry);
		const Vect<uint32_t> playerSize = getPlayerTexSize(registry);

		const auto view = registry.view<Comps::Camera, Comps::Offset>();
		for (const entt::entity entity : view)
		{
			auto [camera, obj] = view.get<Comps::Camera, Comps::Offset>(entity);
			obj.offset += (obj.offset - (playerPos + (playerSize.cast<float>() / 2) - camera.offset)) / camera.followSpeed; // Maybe work?
		}
	}

	const Vect<float> getPlayerPos(entt::registry& registry)
	{
		const auto view = registry.view<Comps::Position, Tags::Player>();
		for (const entt::entity entity : view)
			return view.get<Comps::Position>(entity).pos;

		std::cout << "Unable to find player entity" << std::endl;
		return Vect<float>(); // Oof lol
	}

	const Vect<uint32_t> getPlayerTexSize(entt::registry& registry)
	{
		const auto view = registry.view<Comps::Texture, Tags::Player>();
		for (const entt::entity entity : view)
			return view.get<Comps::Texture>(entity).destSize;

		return Vect<uint32_t>(); // Oof
	}
}