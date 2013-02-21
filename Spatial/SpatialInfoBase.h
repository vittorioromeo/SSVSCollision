#ifndef SPATIALINFOBASE_H
#define SPATIALINFOBASE_H

#include <sparsehash/dense_hash_set>
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
