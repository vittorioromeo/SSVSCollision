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
	void Retro::resolve(Body& mBody, vector<Body*>& mBodiesToResolve)
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
			bool oldShapeLeftOfS{oldShape.isLeftOf(s)}, oldShapeRightOfS{oldShape.isRightOf(s)};
			bool oldShapeAboveS{oldShape.isAbove(s)}, oldShapeBelowS{oldShape.isBelow(s)};
			bool oldHOverlap{!(oldShapeLeftOfS || oldShapeRightOfS)}, oldVOverlap{!(oldShapeAboveS || oldShapeBelowS)};

			if		(resolution.y < 0 && mBody.getVelocity().y > 0 && (oldShapeAboveS || (os.isBelow(shape) && oldHOverlap))) mBody.setVelocityY(0);
			else if	(resolution.y > 0 && mBody.getVelocity().y < 0 && (oldShapeBelowS || (os.isAbove(shape) && oldHOverlap))) mBody.setVelocityY(0);

			if		(resolution.x < 0 && mBody.getVelocity().x > 0 && (oldShapeLeftOfS || (os.isRightOf(shape) && oldVOverlap))) mBody.setVelocityX(0);
			else if	(resolution.x > 0 && mBody.getVelocity().x < 0 && (oldShapeRightOfS || (os.isLeftOf(shape) && oldVOverlap))) mBody.setVelocityX(0);
		}
	}
}

