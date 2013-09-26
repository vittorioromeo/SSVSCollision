// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY_CALLBACKINFO
#define SSVSC_BODY_CALLBACKINFO

#include "SSVSCollision/Global/Typedefs.h"

namespace ssvsc
{
	template<typename> class BodyType;

	template<typename TS> struct DetectionInfo
	{
		BodyType<TS>& body;
		void* userData;
		const Vec2i& intersection;
		float frameTime;
	};

	template<typename TS> struct ResolutionInfo
	{
		BodyType<TS>& body;
		void* userData;
		const Vec2i& intersection;
		const Vec2i& resolution;
		bool& noResolvePosition;
		bool& noResolveVelocity;
	};
}

#endif
