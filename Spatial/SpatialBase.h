#ifndef SPATIALBASE_H
#define SPATIALBASE_H

namespace ssvsc
{
	class Body;
	class SpatialInfoBase;

	class SpatialBase
	{
		public:
			SpatialBase() { }
			virtual ~SpatialBase() { }
			virtual SpatialInfoBase& createSpatialInfo(Body& mBody) = 0;
	};
}

#endif // SPATIALBASE_H
