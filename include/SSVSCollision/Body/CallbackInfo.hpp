// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY_CALLBACKINFO
#define SSVSC_BODY_CALLBACKINFO

namespace ssvsc
{
	template<typename TW> class Body;

	template<typename TW> struct DetectionInfo
	{
		Body<TW>& body;
		void* userData;
		float frameTime;
	};

	template<typename TW> struct ResolutionInfo
	{
		Body<TW>& body;
		void* userData;
		const Vec2i& resolution;
		bool& noResolvePosition;
		bool& noResolveVelocity;
	};
}

#endif
