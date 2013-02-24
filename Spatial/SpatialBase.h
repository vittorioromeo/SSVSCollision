#ifndef SPATIALBASE_H
#define SPATIALBASE_H

namespace ssvsc
{
	class Body;
	class SpatialInfoBase;

	struct SpatialBase
	{
		SpatialBase() { }
		virtual ~SpatialBase() { }
		virtual SpatialInfoBase& createSpatialInfo(Body& mBody) = 0;
		virtual void delSpatialInfo(SpatialInfoBase& mSpatialInfo) = 0;
	};
}

#endif // SPATIALBASE_H
