#ifndef RETRO_H
#define RETRO_H

#include <SFML/Graphics.hpp>
#include "Body/Body.h"
#include "Resolver/ResolverBase.h"
#include "Utils/Utils.h"

namespace ssvsc
{
	struct Retro : public ResolverBase
	{
		void resolve(Body& mBody, std::vector<Body*>& mBodiesToResolve) override;
	};
}

#endif // RETRO_H
