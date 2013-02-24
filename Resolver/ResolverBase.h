#ifndef RESOLVERBASE_H
#define RESOLVERBASE_H

#include <vector>

namespace ssvsc
{
	class AABB;

	struct ResolverBase
	{
		ResolverBase() { }
		virtual ~ResolverBase() { }
		virtual void resolve(Body& mBody, std::vector<Body*>& mBodiesToResolve) = 0;
	};
}

#endif // RESOLVERBASE_H
