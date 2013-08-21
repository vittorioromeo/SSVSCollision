// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_INFOBASE
#define SSVSC_SPATIAL_INFOBASE

#include <SSVUtils/SSVUtils.h>
#include "SSVSCollision/Spatial/SpatialBase.h"

namespace ssvsc
{
	class Base;
	class Body;

	class SpatialInfoBase : public ssvu::MemoryManageable
	{
		protected:
			SpatialBase& spatial;
			Base& base;

		public:
			SpatialInfoBase(SpatialBase& mSpatial, Base& mBase) : spatial(mSpatial), base(mBase) { }
			inline virtual ~SpatialInfoBase() { }

			virtual void invalidate() = 0;
			virtual void preUpdate() = 0;
			virtual void postUpdate() = 0;
			virtual void handleCollisions(float mFrameTime) = 0;
			inline virtual void destroy() { spatial.del(*this); }
	};
}

#endif
