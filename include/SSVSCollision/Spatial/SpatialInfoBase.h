// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_INFOBASE
#define SSVSC_SPATIAL_INFOBASE

#include "SSVSCollision/Spatial/SpatialBase.h"

namespace ssvsc
{
	class Base;
	class Body;

	class SpatialInfoBase
	{
		protected:
			SpatialBase& spatial;
			Base& base;
			bool alive{true};

		public:
			SpatialInfoBase(SpatialBase& mSpatial, Base& mBase) : spatial(mSpatial), base(mBase) { }
			virtual ~SpatialInfoBase() { }

			inline bool isAlive() const { return alive; }

			virtual void invalidate() = 0;
			virtual void preUpdate() = 0;
			virtual void postUpdate() = 0;
			virtual void destroy() { alive = false; }
			virtual void handleCollisions(float mFrameTime) = 0;
	};
}

#endif
