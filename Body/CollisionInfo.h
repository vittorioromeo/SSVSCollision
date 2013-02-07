// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef COLLISIONINFO_H
#define COLLISIONINFO_H

namespace ssvsc
{
	class Body;
	struct CollisionInfo { Body* body; float frameTime; void* userData; };
}

#endif // COLLISIONINFO_H
