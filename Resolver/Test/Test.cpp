#include "Test.h"

using namespace std;
using namespace sf;
using namespace ssvsc::Utils;

namespace ssvsc
{
	void Test::resolve(Body& mBody, std::vector<AABB>& mShapesToResolve)
	{
		AABB& shape(mBody.getShape());
		sort(mShapesToResolve, [&](const AABB& mA, const AABB& mB){ return getOverlapArea(shape, mA) > getOverlapArea(shape, mB); });

		for(auto& s : mShapesToResolve)
		{
			Vector2i minIntersection{getMin1DIntersection(shape, s)};
			mBody.onResolution({s, minIntersection});
			shape.move(minIntersection);
			getOverlapX(shape, s) > getOverlapY(shape, s) ? mBody.setVelocityY(0) : mBody.setVelocityX(0);
		}
	}
}

