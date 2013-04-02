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
		for(auto group : mBody->getGroups()) groupedBodies[group].push_back(mBody);
	}
	void Cell::del(Body* mBody)
	{
		eraseRemove(bodies, mBody);
		for(auto group : mBody->getGroups()) eraseRemove(groupedBodies[group], mBody);
	}

	vector<Body*>& Cell::getBodies() { return bodies; }
	vector<Body*>& Cell::getBodies(const string& mGroup) { return groupedBodies[mGroup]; }
	int Cell::getLeft() const	{ return left; }
	int Cell::getRight() const 	{ return right; }
	int Cell::getTop() const 	{ return top; }
	int Cell::getBottom() const { return bottom; }
	bool Cell::hasGroup(const string& mGroup) const { return groupedBodies.count(mGroup) > 0; }
}

