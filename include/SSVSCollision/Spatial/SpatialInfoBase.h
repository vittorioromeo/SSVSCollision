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

	class SpatialInfoBase : public ssvu::MemoryManageable
	{
		protected:
			SpatialBase& spatial;
			Base& base;

		public:
			int spatialPaint{-1};

			SpatialInfoBase(SpatialBase& mSpatial, Base& mBase) : spatial(mSpatial), base(mBase) { }
			inline virtual ~SpatialInfoBase() { }

			virtual void init() = 0;
			virtual void invalidate() = 0;
			virtual void preUpdate() = 0;
			virtual void postUpdate() = 0;
			virtual void handleCollisions(float mFT) = 0;
			inline virtual void destroy() { spatial.del(*this); }
	};
}

#endif
