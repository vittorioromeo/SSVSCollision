// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SPATIALINFOBASE_H
#define SPATIALINFOBASE_H

#include <google/dense_hash_set>
#include "Spatial/SpatialBase.h"

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

#endif // SPATIALINFOBASE_H
