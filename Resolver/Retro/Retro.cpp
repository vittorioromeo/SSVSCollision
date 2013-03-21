// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <SSVUtils/SSVUtils.h>
#include "Resolver/Retro/Retro.h"
#include "Utils/Utils.h"

using namespace std;
using namespace sf;
using namespace ssvsc::Utils;
using namespace ssvu;

namespace ssvsc
{
	void Retro::resolve(Body& mBody, std::vector<Body*>& mBodiesToResolve)
	{
		AABB& shape(mBody.getShape());
		const AABB& oldShape(mBody.getOldShape());
		sort(mBodiesToResolve, [&](Body* mA, Body* mB){ return getOverlapArea(shape, mA->getShape()) > getOverlapArea(shape, mB->getShape()); });

		for(auto& b : mBodiesToResolve)
		{
			bool noResolvePosition{false}, noResolveVelocity{false};

			const AABB& s(b->getShape());
			const AABB& os(b->getOldShape());
			Vector2i resolution{getMin1DIntersection(shape, s)};
			mBody.onResolution({*b, b->getUserData(), resolution, noResolvePosition, noResolveVelocity});

			if(!noResolvePosition) shape.move(resolution);
			if(noResolveVelocity) continue;

			// Remember that shape has moved now

			if(resolution.y < 0 && mBody.getVelocity().y > 0)
			{
				if(oldShape.isAbove(s) || (os.isBelow(shape) && !(oldShape.isLeftOf(s) || oldShape.isRightOf(s))))
					mBody.setVelocityY(0);
			}
			else if(resolution.y > 0 && mBody.getVelocity().y < 0)
			{
				if(oldShape.isBelow(s) || (os.isAbove(shape) && !(oldShape.isLeftOf(s) || oldShape.isRightOf(s))))
					mBody.setVelocityY(0);
			}

			if(resolution.x < 0 && mBody.getVelocity().x > 0)
			{
				if(oldShape.isLeftOf(s) || (os.isRightOf(shape) && !(oldShape.isAbove(s) || oldShape.isBelow(s))))
					mBody.setVelocityX(0);
			}
			else if(resolution.x > 0 && mBody.getVelocity().x < 0)
			{
				if(oldShape.isRightOf(s) || (os.isLeftOf(shape) && !(oldShape.isAbove(s) || oldShape.isBelow(s))))
					mBody.setVelocityX(0);
			}
		}
	}
}

