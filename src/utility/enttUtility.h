#pragma once

#include "entt/entt.hpp"

namespace Utility
{
	template <typename Comp, typename Tag>
	inline Comp getEnttComp(entt::registry& registry)
	{
		const auto view = registry.view<Comp, Tag>();
		for (const entt::entity entity : view)
			return view.get<Comp>(entity);

		std::cout << "Couldn't find object" << std::endl;
		return Comp();
	}
}