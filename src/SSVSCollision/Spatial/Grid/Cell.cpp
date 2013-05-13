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
	Cell::Cell(int mLeft, int mRight, int mTop, int mBottom) : left{mLeft}, right{mRight}, top{mTop}, bottom{mBottom} { }

	void Cell::add(Body* mBody)
	{
		bodies.push_back(mBody);
		for(const auto& groupId : mBody->getGroupIds()) groupedBodies[groupId].push_back(mBody);
	}
	void Cell::del(Body* mBody)
	{
		eraseRemove(bodies, mBody);
		for(const auto& groupId : mBody->getGroupIds()) eraseRemove(groupedBodies[groupId], mBody);
	}
}

