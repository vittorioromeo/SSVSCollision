// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIALBASE
#define SSVSC_SPATIALBASE

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

#endif
