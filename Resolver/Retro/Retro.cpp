#include "Retro.h"

using namespace std;
using namespace sf;
using namespace ssvsc::Utils;

namespace ssvsc
{
	void Retro::resolve(Body& mBody, std::vector<Body*>& mBodiesToResolve)
	{
		AABB& shape(mBody.getShape());
		const AABB& oldShape(mBody.getOldShape());
		sort(mBodiesToResolve, [&](Body* mA, Body* mB){ return getOverlapArea(shape, mA->getShape()) > getOverlapArea(shape, mB->getShape()); });

		for(auto& b : mBodiesToResolve)
		{
			const AABB& s(b->getShape());
			Vector2i minIntersection{getMin1DIntersection(shape, s)};
			mBody.onResolution({*b, minIntersection});
			shape.move(minIntersection);
			//getOverlapX(shape, s) > getOverlapY(shape, s) ? mBody.setVelocityY(0) : mBody.setVelocityX(0);

			if(oldShape.isAbove(s) && minIntersection.y < 0) mBody.setVelocityY(0);
			else if(oldShape.isBelow(s) && minIntersection.y > 0) mBody.setVelocityY(0);

			if(oldShape.isLeftOf(s) && minIntersection.x < 0) mBody.setVelocityX(0);
			else if(oldShape.isRightOf(s) && minIntersection.x > 0) mBody.setVelocityX(0);
		}
	}
}

