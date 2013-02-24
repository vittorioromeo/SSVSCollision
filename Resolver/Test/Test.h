#ifndef TEST_H
#define TEST_H

#include <SFML/Graphics.hpp>
#include "Body/Body.h"
#include "Resolver/ResolverBase.h"
#include "Utils/Utils.h"

namespace ssvsc
{
	class Test : public ResolverBase
	{
		public:
			void resolve(Body& mBody, std::vector<Body*>& mBodiesToResolve) override;
	};
}

#endif // TEST_H
