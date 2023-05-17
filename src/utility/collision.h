#pragma once

#include <SDL2/SDL.h>
#include "utility/vect.h"
#include "comps/hitbox.h"

namespace Utility
{
	inline const bool collision(const SDL_Rect& rect1, const SDL_Rect& rect2)
    {
        return (rect1.x < rect2.x + rect2.w &&
                rect1.y < rect2.y + rect2.h &&
                rect2.x < rect1.x + rect1.w &&
                rect2.y < rect1.y + rect1.h);
    }

    inline const bool collision(const Vect<int32_t> pos1, const Vect<uint32_t> size1, 
                                const Vect<int32_t> pos2, const Vect<uint32_t> size2)
    {
        return collision(Vect<int32_t>::toRect(pos1, size1.cast<int32_t>()), Vect<int32_t>::toRect(pos2, size2.cast<int32_t>()));
    }

    inline const bool collision(const Comps::Box& box1, const Comps::Box& box2)
    {
        return collision(Vect<float>::toRect(box1.offset.offset, box1.size.cast<float>()),
                         Vect<float>::toRect(box2.offset.offset, box2.size.cast<float>()));
    }
}