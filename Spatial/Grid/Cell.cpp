// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <SSVUtils.h>
#include "Body/Body.h"
#include "Cell.h"

using namespace std;
using namespace ssvu::Utils;

namespace ssvsc
{
	Cell::Cell(int mLeft, int mRight, int mTop, int mBottom) : left{mLeft}, right{mRight}, top{mTop}, bottom{mBottom} { }

	void Cell::add(Body* mBody) { for(auto group : mBody->getGroups()) bodies[group].push_back(mBody); }
	void Cell::del(Body* mBody) { for(auto group : mBody->getGroups()) eraseRemove(bodies[group], mBody); }

	vector<Body*>* Cell::getQuery(const string& mGroup) { return &bodies[mGroup]; }
	int Cell::getLeft() const	{ return left; }
	int Cell::getRight() const 	{ return right; }
	int Cell::getTop() const 	{ return top; }
	int Cell::getBottom() const { return bottom; }
	
	bool Cell::hasGroup(const string& mGroup) const { return bodies.count(mGroup) > 0; }
}

