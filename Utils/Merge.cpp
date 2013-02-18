#include "Merge.h"

using namespace std;
using namespace sf;

namespace ssvsc
{
	namespace Utils
	{
		AABB getMergedAABBHorizontally(const AABB& mA, const AABB& mB)
		{
			int x{min(mA.getX(), mB.getX()) + abs(mA.getX() - mB.getX())};
			Vector2i halfSize{mA.getHalfWidth() + mB.getHalfWidth(), mA.getHalfHeight()};
			return {{x, mA.getY()}, halfSize};
		}

		AABB getMergedAABBVertically(const AABB& mA, const AABB& mB)
		{
			int y{min(mA.getY(), mB.getY()) + abs(mA.getY() - mB.getY())};
			Vector2i halfSize{mA.getHalfWidth(), mA.getHalfHeight() + mB.getHalfHeight()};
			return {{mA.getX(), y}, halfSize};
		}
	}
}

