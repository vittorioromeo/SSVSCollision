// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIALINFOBASE
#define SSVSC_SPATIALINFOBASE

#include "SSVSCollision/Spatial/SpatialBase.h"
#pragma GCC system_header
#include <google/dense_hash_set>

namespace ssvsc
{
	class Body;

	class SpatialInfoBase
	{
		private:
			SpatialBase& spatial;

		public:
			Body& body;

			SpatialInfoBase(SpatialBase& mSpatial, Body& mBody) : spatial(mSpatial), body(mBody) { }
			virtual ~SpatialInfoBase() { }

			virtual void invalidate() = 0;
			virtual void preUpdate() = 0;
			virtual void postUpdate() = 0;
			virtual google::dense_hash_set<Body*> getBodiesToCheck() = 0;
			void destroy() { spatial.delSpatialInfo(*this); }
	};
}

#endif
