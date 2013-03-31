// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <SSVStart/SSVStart.h>
#include "SSVSCollision/Spatial/Grid/GridQuery.h"
#include "SSVSCollision/Spatial/Grid/GridInfo.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"

using namespace std;
using namespace sf;
using namespace ssvu;

namespace ssvsc
{
	float Signed2DTriArea(Vector2f a, Vector2f b, Vector2f c)
	{
		return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
	}

	bool Test2DSegmentSegment(Vector2f a, Vector2f b, Vector2f c, Vector2f d, float &t, Vector2f &p)
	{
		// Sign of areas correspond to which side of ab points c and d are
		float a1 = Signed2DTriArea(a, b, d); // Compute winding of abd (+ or -)
		float a2 = Signed2DTriArea(a, b, c); // To intersect, must have sign opposite of a1
		// If c and d are on different sides of ab, areas have different signs
		if (a1 * a2 < 0.0f) {
		// Compute signs for a and b with respect to segment cd
		float a3 = Signed2DTriArea(c, d, a); // Compute winding of cda (+ or -)
		// Since area is constant a1 - a2 = a3 - a4, or a4 = a3 + a2 - a1
		// float a4 = Signed2DTriArea(c, d, b); // Must have opposite sign of a3
		float a4 = a3 + a2 - a1;
		// Points a and b on different sides of cd if areas have different signs
		if (a3 * a4 < 0.0f) {
		// Segments intersect. Find intersection point along L(t) = a + t * (b - a).
		// Given height h1 of an over cd and height h2 of b over cd,
		// t = h1 / (h1 - h2) = (b*h1/2) / (b*h1/2 - b*h2/2) = a3 / (a3 - a4),
		// where b (the base of the triangles cda and cdb, i.e., the length
		// of cd) cancels out.
		t = a3 / (a3 - a4);
		p = a + t * (b - a);
		return true;
		}
		}
		// Segments not intersecting (or collinear)
		return false;
	}

	Vector2f lineIntersection(Vector2f mA1, Vector2f mA2, Vector2f mB1, Vector2f mB2)
	{
		float d = (mA1.x - mA2.x) * (mB1.y - mB2.y) - (mA1.y - mA2.y) * (mB1.x - mB2.x);
		// If d is zero, there is no intersection
		if (d == 0) return Vector2f(0, 0);

		// Get the x and y
		float pre = (mA1.x*mA2.y - mA1.y*mA2.x), post = (mB1.x*mB2.y - mB1.y*mB2.x);
		float x = ( pre * (mB1.x - mB2.x) - (mA1.x - mA2.x) * post ) / d;
		float y = ( pre * (mB1.y - mB2.y) - (mA1.y - mA2.y) * post ) / d;

		// Check if the x and y coordinates are within both lines
		if ( x < min(mA1.x, mA2.x) || x > max(mA1.x, mA2.x) ||
		x < min(mB1.x, mB2.x) || x > max(mB1.x, mB2.x) ) return Vector2f(0, 0);
		if ( y < min(mA1.y, mA2.y) || y > max(mA1.y, mA2.y) ||
		y < min(mB1.y, mB2.y) || y > max(mB1.y, mB2.y) ) return Vector2f(0, 0);

		return Vector2f(x, y);
	}

	GridQuery::GridQuery(Grid& mGrid, Vector2i mStartPos) : grid(mGrid), startPos{Vector2f(mStartPos)}, pos{startPos},
		startIndex{grid.getIndex(mStartPos)}, index{startIndex}, direction(Vector2f(5.f, -2.f)) { }

	Body*GridQuery::nextByDirection(Vector2f mDirection)
	{
		direction = ssvs::Utils::getNormalized(mDirection);
		deltaDist = {(float)sqrt(1 + (direction.y * direction.y) / (direction.x * direction.x)), (float)sqrt(1 + (direction.x * direction.x) / (direction.y * direction.y))};
		return nextImpl<QueryTraits::RayCast, QueryTraits::Bodies::AllOffset>();
	}
	Body*GridQuery::nextByDirection(Vector2f mDirection, const string& mGroup)
	{
		direction = ssvs::Utils::getNormalized(mDirection);
		deltaDist = {(float)sqrt(1 + (direction.y * direction.y) / (direction.x * direction.x)), (float)sqrt(1 + (direction.x * direction.x) / (direction.y * direction.y))};
		return nextImpl<QueryTraits::RayCast, QueryTraits::Bodies::GroupedOffset>(mGroup);
	}

	void GridQuery::reset()
	{
		pos = startPos;
		index = startIndex;
		deltaDist = {(float)sqrt(1 + (direction.y * direction.y) / (direction.x * direction.x)), (float)sqrt(1 + (direction.x * direction.x) / (direction.y * direction.y))};
		bodies.clear();
	}

	// Getters
	Grid& GridQuery::getGrid() const			{ return grid; }
	Vector2f GridQuery::getStartPos() const		{ return startPos; }
	Vector2i GridQuery::getStartIndex() const	{ return startIndex; }
	Vector2f GridQuery::getPos() const			{ return pos; }
	Vector2f GridQuery::getOut() const			{ return out; }
	Vector2i GridQuery::getIndex() const		{ return index; }
	Vector2f GridQuery::getDirection() const	{ return direction; }
	Vector2i GridQuery::getStep() const			{ return step; }
	Vector2f GridQuery::getDeltaDist() const	{ return deltaDist; }
	Vector2f GridQuery::getSideDist() const		{ return sideDist; }

	// Setters
	void GridQuery::setPos(Vector2f mPos)		{ pos = mPos; }
	void GridQuery::setIndexX(int mIndexX)		{ index.x = mIndexX; }
	void GridQuery::setIndexY(int mIndexY)		{ index.y = mIndexY; }
	void GridQuery::setOutX(float mOutX)		{ out.x = mOutX; }
	void GridQuery::setOutY(float mOutY)		{ out.y = mOutY; }
	void GridQuery::setStepX(int mStepX)		{ step.x = mStepX; }
	void GridQuery::setStepY(int mStepY)		{ step.y = mStepY; }
	void GridQuery::setSideDistX(float mSideDistX){ sideDist.x = mSideDistX; }
	void GridQuery::setSideDistY(float mSideDistY){ sideDist.y = mSideDistY; }
}
