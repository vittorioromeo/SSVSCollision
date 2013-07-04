// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_RESOLVER_BASE
#define SSVSC_RESOLVER_BASE

#include <vector>

namespace ssvsc
{
	class Body;

	struct ResolverBase
	{
		ResolverBase() { }
		virtual ~ResolverBase() { }
		virtual void resolve(Body& mBody, std::vector<Body*>& mBodiesToResolve) = 0;
	};
}

#endif
