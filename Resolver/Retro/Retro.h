// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

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
