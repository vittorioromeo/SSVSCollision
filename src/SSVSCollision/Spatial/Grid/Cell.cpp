// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <SSVUtils/SSVUtils.h>
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"
#include "SSVSCollision/Spatial/Grid/GridInfo.h"

using namespace std;
using namespace ssvu;

namespace ssvsc
{
	Cell::Cell(int mLeft, int mRight, int mTop, int mBottom) : aabb{mLeft, mRight, mTop, mBottom} { }

	void Cell::add(Base* mBase, Type mType)
	{
		bases.push_back(mBase);
		for(auto& e : bases) static_cast<GridInfo&>(e->getSpatialInfo()).mustGather = true;

		if(mType == Type::Body) addBody(static_cast<Body*>(mBase));
	}
	void Cell::del(Base* mBase, Type mType)
	{
		eraseRemove(bases, mBase);

		if(mType == Type::Body) delBody(static_cast<Body*>(mBase));
	}
	void Cell::addBody(Body* mBody)
	{
		bodies.push_back(mBody);
		for(const auto& uid : mBody->getGroupUids()) groupedBodies[uid].push_back(mBody);
	}
	void Cell::delBody(Body* mBody)
	{
		eraseRemove(bodies, mBody);
		for(const auto& uid : mBody->getGroupUids()) eraseRemove(groupedBodies[uid], mBody);
	}
}

