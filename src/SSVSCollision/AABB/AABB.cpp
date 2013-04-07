// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/AABB/AABB.h"

using namespace sf;

namespace ssvsc
{
	AABB::AABB(Vector2i mPosition, Vector2i mHalfSize) : position{mPosition}, halfSize{mHalfSize} { }
}
