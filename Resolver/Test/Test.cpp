#include "Test.h"

using namespace std;
using namespace sf;
using namespace ssvsc::Utils;

namespace ssvsc
{
	void Test::resolve(Body& mBody, const std::vector<AABB>& mShapesToResolve)
	{
		for(auto& s : mShapesToResolve)
		{
			AABB& shape(mBody.getShape());
			
			Vector2i minIntersection{getMin1DIntersection(shape, s)};
			mBody.onResolution({s, minIntersection});
			shape.move(minIntersection);
			getOverlapX(shape, s) > getOverlapY(shape, s) ? mBody.setVelocityY(0) : mBody.setVelocityX(0);
		}
	}
}

