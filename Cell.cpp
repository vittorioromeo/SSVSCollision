#include "Cell.h"
#include "Body.h"

using namespace std;
using namespace ssvs::Utils;

namespace ssvsc
{
	Cell::Cell(int mLeft, int mRight, int mTop, int mBottom) : left{mLeft}, right{mRight}, top{mTop}, bottom{mBottom} { }

	void Cell::addBody(Body* mBody) { for(auto group : mBody->getGroups()) groupedBodies[group].push_back(mBody); }
	void Cell::delBody(Body* mBody) { for(auto group : mBody->getGroups()) eraseFromVector(groupedBodies[group], mBody); }
	bool Cell::hasGroup(const string& mGroup) { return groupedBodies.count(mGroup) > 0; }

	// Getters
	vector<Body*> Cell::getGroupedBodies(string mGroup) { return groupedBodies[mGroup]; }
	int Cell::getLeft() { return left; }
	int Cell::getRight() { return right; }
	int Cell::getTop() { return top; }
	int Cell::getBottom() { return bottom; }
}

