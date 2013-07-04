// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BASE
#define SSVSC_BASE

#include <bitset>
#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/World/World.h"

namespace ssvsc
{
	class AABB;
	class Body;
	class SpatialInfoBase;

	class Base
	{
		protected:
			World& world;
			SpatialInfoBase& spatialInfo;
			bool outOfBounds{false};

			Base(World& mWorld) : world(mWorld), spatialInfo(world.getSpatial().createSpatialInfo(*this)) { }

		public:
			virtual ~Base() { }

			virtual	void update(float mFrameTime) = 0;
			virtual void handleCollision(float mFrameTime, Body* mBody) = 0;
			virtual AABB& getShape() = 0;
			virtual AABB& getOldShape() = 0;
			virtual Type getType() = 0;
			virtual void destroy() { world.delBase(this); }

			inline SpatialInfoBase& getSpatialInfo()		{ return spatialInfo; }
			inline void setOutOfBounds(bool mOutOfBounds)	{ outOfBounds = mOutOfBounds; }
	};
}

#endif
