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
			Vector2i minIntersection{getMin1DIntersection(mBody.getShape(), s)};
			mBody.onResolution({s, minIntersection});
			mBody.getShape().move(minIntersection);
			if(getOverlapX(mBody.getShape(), s) > getOverlapY(mBody.getShape(), s)) mBody.setVelocityY(0);
			else mBody.setVelocityX(0);
		}
	}
}

