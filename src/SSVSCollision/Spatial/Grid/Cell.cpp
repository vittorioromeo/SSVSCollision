// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <SSVUtils/SSVUtils.h>
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"

using namespace std;
using namespace ssvu;

namespace ssvsc
{
	Cell::Cell(int mLeft, int mRight, int mTop, int mBottom) : aabb{mLeft, mRight, mTop, mBottom} { }

	void Cell::add(Body* mBody)
	{
		bodies.push_back(mBody);
		for(const auto& uid : mBody->getGroupUids()) groupedBodies[uid].push_back(mBody);
	}
	void Cell::del(Body* mBody)
	{
		eraseRemove(bodies, mBody);
		for(const auto& uid : mBody->getGroupUids()) eraseRemove(groupedBodies[uid], mBody);
	}
}

