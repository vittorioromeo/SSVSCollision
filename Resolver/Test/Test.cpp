#include "Test.h"

using namespace std;
using namespace sf;
using namespace ssvsc::Utils;

namespace ssvsc
{
	void Test::resolve(Body& mBody, std::vector<Body*>& mBodiesToResolve)
	{
		AABB& shape(mBody.getShape());
		sort(mBodiesToResolve, [&](Body* mA, Body* mB){ return getOverlapArea(shape, mA->getShape()) > getOverlapArea(shape, mB->getShape()); });

		for(auto& b : mBodiesToResolve)
		{
			AABB& s(b->getShape());
			Vector2i minIntersection{getMin1DIntersection(shape, s)};
			mBody.onResolution({s, minIntersection});
			shape.move(minIntersection);
			getOverlapX(shape, s) > getOverlapY(shape, s) ? mBody.setVelocityY(0) : mBody.setVelocityX(0);
		}
	}
}

