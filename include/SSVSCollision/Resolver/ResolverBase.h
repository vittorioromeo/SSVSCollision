// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_RESOLVER_BASE
#define SSVSC_RESOLVER_BASE

#include <vector>

namespace ssvsc
{
	template<typename> class BodyType;
	template<typename> class World;

	template<typename TS> struct ResolverBase
	{
		virtual ~ResolverBase() { }
		virtual void resolve(float mFT, BodyType<TS>& mBody, std::vector<BodyType<TS>*>& mBodiesToResolve) = 0;
		virtual void postUpdate(World<TS>&) { }
	};
}

#endif
