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

	void Cell::add(Base* mBase)		{ if(mBase->getType() == Type::Body) addBody(static_cast<Body*>(mBase)); }
	void Cell::del(Base* mBase)		{ if(mBase->getType() == Type::Body) delBody(static_cast<Body*>(mBase)); }
	void Cell::addBody(Body* mBody)	{ bodies.push_back(mBody); }
	void Cell::delBody(Body* mBody)	{ eraseRemove(bodies, mBody); }
}

