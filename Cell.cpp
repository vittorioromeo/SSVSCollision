#include "Cell.h"
#include "Body.h"

using namespace std;
using namespace ssvs::Utils;

namespace ssvsc
{
	Cell::Cell(int mLeft, int mRight, int mTop, int mBottom) : left{mLeft}, right{mRight}, top{mTop}, bottom{mBottom} { }

	void Cell::add(Body* mBody)
	{
		for(auto group : mBody->getGroups()) bodies[group].push_back(mBody);
		for(auto group : mBody->getGroupsToCheck()) mBody->queries.push_back(&bodies[group]); 
	}
	void Cell::del(Body* mBody)
	{
		for(auto group : mBody->getGroups()) eraseFromVector(bodies[group], mBody);
		for(auto group : mBody->getGroupsToCheck()) eraseFromVector(mBody->queries, &bodies[group]);
	}
	bool Cell::hasGroup(const string& mGroup) { return bodies.count(mGroup) > 0; }

	// Properties
	const vector<Body*>& Cell::getBodies(const string& mGroup) { return bodies[mGroup]; }
	int Cell::getLeft() 	{ return left; }
	int Cell::getRight() 	{ return right; }
	int Cell::getTop() 		{ return top; }
	int Cell::getBottom() 	{ return bottom; }
}

