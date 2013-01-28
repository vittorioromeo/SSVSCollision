#include "Body/Body.h"
#include "Cell.h"
#include "Utils/Utils.h"

using namespace std;
using namespace ssvsc::Utils;

namespace ssvsc
{
	Cell::Cell(int mLeft, int mRight, int mTop, int mBottom) : left{mLeft}, right{mRight}, top{mTop}, bottom{mBottom} { }

	void Cell::add(Body* mBody) { for(auto group : mBody->getGroups()) bodies[group].push_back(mBody); }
	void Cell::del(Body* mBody) { for(auto group : mBody->getGroups()) eraseRemove(bodies[group], mBody); }
	bool Cell::hasGroup(const string& mGroup) { return bodies.count(mGroup) > 0; }

	// Properties
	vector<Body*>* Cell::getQuery(const string& mGroup) { return &bodies[mGroup]; }
	int Cell::getLeft() 	{ return left; }
	int Cell::getRight() 	{ return right; }
	int Cell::getTop() 		{ return top; }
	int Cell::getBottom() 	{ return bottom; }
}

