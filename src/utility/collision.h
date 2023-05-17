#pragma once

#include <SDL2/SDL.h>
#include "utility/vect.h"
#include "comps/hitbox.h"

namespace Utility
{
    template <typename T> 
    inline const bool collision(const Vect<T> pos1, const Vect<T> size1,
                                const Vect<T> pos2, const Vect<T> size2)
    {
        return (pos1.x < pos2.x + size2.x &&
                pos1.y < pos2.y + size2.y &&
                pos2.x < pos1.x + size1.x &&
                pos2.y < pos1.y + size1.y);
    }

    inline const bool collision(const Comps::Box& box1, const Comps::Box& box2)
    {
        return collision<float>(box1.offset.offset, box1.size.cast<float>(),
                                box2.offset.offset, box2.size.cast<float>());
    }
}