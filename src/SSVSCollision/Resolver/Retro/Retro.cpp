// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <SSVUtils/SSVUtils.h>
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Resolver/Retro/Retro.h"
#include "SSVSCollision/Utils/Utils.h"
#include "SSVSCollision/Global/Typedefs.h"

using namespace std;
using namespace ssvsc::Utils;
using namespace ssvu;

namespace ssvsc
{
	void Retro::resolve(Body& mBody, std::vector<Body*>& mBodiesToResolve)
	{
		AABB& shape(mBody.getShape());
		const AABB& oldShape(mBody.getOldShape());
		sort(mBodiesToResolve, [&](Body* mA, Body* mB){ return getOverlapArea(shape, mA->getShape()) > getOverlapArea(shape, mB->getShape()); });

		for(const auto& b : mBodiesToResolve)
		{
			bool noResolvePosition{false}, noResolveVelocity{false};

			const AABB& s(b->getShape());
			const AABB& os(b->getOldShape());

			int iX{getMinIntersectionX(shape, s)}, iY{getMinIntersectionY(shape, s)};
			Vec2i intersection({iX, iY});
			Vec2i resolution{abs(iX) < abs(iY) ? Vec2i{iX, 0} : Vec2i{0, iY}};

			mBody.onResolution({*b, b->getUserData(), intersection, resolution, noResolvePosition, noResolveVelocity});

			if(!noResolvePosition) shape.move(resolution);
			if(noResolveVelocity) continue;

			// Remember that shape has moved now
			bool oldHOverlap{!(oldShape.isLeftOf(s) || oldShape.isRightOf(s))}, oldVOverlap{!(oldShape.isAbove(s) || oldShape.isBelow(s))};

			if(resolution.y < 0 && mBody.getVelocity().y > 0)
			{
				if(oldShape.isAbove(s) || (os.isBelow(shape) && oldHOverlap)) mBody.setVelocityY(0);
			}
			else if(resolution.y > 0 && mBody.getVelocity().y < 0)
			{
				if(oldShape.isBelow(s) || (os.isAbove(shape) && oldHOverlap)) mBody.setVelocityY(0);
			}

			if(resolution.x < 0 && mBody.getVelocity().x > 0)
			{
				if(oldShape.isLeftOf(s) || (os.isRightOf(shape) && oldVOverlap)) mBody.setVelocityX(0);
			}
			else if(resolution.x > 0 && mBody.getVelocity().x < 0)
			{
				if(oldShape.isRightOf(s) || (os.isLeftOf(shape) && oldVOverlap)) mBody.setVelocityX(0);
			}
		}
	}
}

