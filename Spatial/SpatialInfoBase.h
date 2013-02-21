#ifndef SPATIALINFOBASE_H
#define SPATIALINFOBASE_H

#include <sparsehash/dense_hash_set>

namespace ssvsc
{
	class Body;

	class SpatialInfoBase
	{
		public:
			SpatialInfoBase() { }
			virtual ~SpatialInfoBase() { }

			virtual void invalidate() = 0;
			virtual void preUpdate() = 0;
			virtual void postUpdate() = 0;
			virtual google::dense_hash_set<Body*> getBodiesToCheck() = 0;
			virtual void destroy() = 0;
	};
}

#endif // SPATIALINFOBASE_H
