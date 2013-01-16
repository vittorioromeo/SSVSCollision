#ifndef COLLISIONINFO_H
#define COLLISIONINFO_H

namespace ssvsc
{
	class Body;
	struct CollisionInfo { Body* body; float frameTime; void* userData; };
}

#endif // COLLISIONINFO_H
