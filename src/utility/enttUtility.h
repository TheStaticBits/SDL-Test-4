#pragma once

#include "entt/entt.hpp"

namespace Utility
{
	template <typename Comp, typename Tag>
	inline const Comp* getEnttComp(entt::registry& registry)
	{
		const auto view = registry.view<Comp, Tag>();
		for (const entt::entity entity : view)
			return &view.template get<Comp>(entity);

		std::cout << "Couldn't find object" << std::endl;
		return nullptr;
	}
}