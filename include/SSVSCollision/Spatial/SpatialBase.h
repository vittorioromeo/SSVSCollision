// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_BASE
#define SSVSC_SPATIAL_BASE

namespace ssvsc
{
	class Base;
	class SpatialInfoBase;

	struct SpatialBase
	{
		SpatialBase() { }
		virtual ~SpatialBase() { }
		virtual SpatialInfoBase& createSpatialInfo(Base& mBase) = 0;
		virtual void refresh() = 0;
	};
}

#endif
