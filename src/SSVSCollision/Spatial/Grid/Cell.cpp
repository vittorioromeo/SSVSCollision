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
	Cell::Cell(const AABB& mAABB) : aabb{mAABB} { }

	void Cell::add(Base* mBase)
	{
		addBase(mBase);
		if(mBase->getType() == Type::Body) addBody(static_cast<Body*>(mBase));
	}
	void Cell::del(Base* mBase)
	{
		delBase(mBase);
		if(mBase->getType() == Type::Body) delBody(static_cast<Body*>(mBase));
	}

	void Cell::addBase(Base* mBase)
	{
		bases.push_back(mBase);
		for(auto& e : bases) static_cast<GridInfo&>(e->getSpatialInfo()).mustGather = true;
	}
	void Cell::delBase(Base* mBase)
	{
		eraseRemove(bases, mBase);
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

