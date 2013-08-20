// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRIDQUERYTYPES
#define SSVSC_SPATIAL_GRIDQUERYTYPES

#include <vector>
#include <queue>
#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Spatial/Grid/GridQuery.h"

namespace ssvsc
{
	namespace GridQueryTypes
	{
		template<typename TDerived> struct Base
		{
			GridQuery<TDerived>& query;
			Base(GridQuery<TDerived>& mQuery) : query(mQuery) { }
		};

		namespace Bodies
		{
			struct All
			{
				inline static void getBodies(std::vector<Body*>& mBodies, Grid& mGrid, Vec2i& mIndex, Group) { mBodies = mGrid.getCell(mIndex).getBodies(); }
			};
			struct Grouped
			{
				inline static void getBodies(std::vector<Body*>& mBodies, Grid& mGrid, Vec2i& mIndex, Group mGroup)
				{
					// TODO: improve
					std::vector<Body*> temp;
					for(const auto& b : mGrid.getCell(mIndex).getBodies()) if(b->hasGroup(mGroup)) temp.push_back(b);
					mBodies = temp;
				}
			};
		}

		namespace Orthogonal
		{
			struct Left : public Base<Left>
			{
				Left(GridQuery<Left>& mQuery) : Base{mQuery}			{ }
				inline bool isValid()									{ return query.index.x >= query.grid.getIndexXMin(); }
				inline void step()										{ --query.index.x; }
				inline bool getSorting(const Body* mA, const Body* mB)	{ return mA->getPosition().x < mB->getPosition().x; }
				inline bool misses(const AABB& mShape)					{ return mShape.getLeft() > query.pos.x || query.pos.y < mShape.getTop() || query.pos.y > mShape.getBottom(); }
				inline void setOut(const AABB& mShape)					{ query.lastPos = Vec2f(mShape.getRight(), query.pos.y); }
			};
			struct Right : public Base<Right>
			{
				Right(GridQuery<Right>& mQuery) : Base{mQuery}			{ }
				inline bool isValid()									{ return query.index.x < query.grid.getIndexXMax(); }
				inline void step()										{ ++query.index.x; }
				inline bool getSorting(const Body* mA, const Body* mB)	{ return mA->getPosition().x > mB->getPosition().x; }
				inline bool misses(const AABB& mShape)					{ return mShape.getRight() < query.pos.x || query.pos.y < mShape.getTop() || query.pos.y > mShape.getBottom(); }
				inline void setOut(const AABB& mShape)					{ query.lastPos = Vec2f(mShape.getLeft(), query.pos.y); }
			};
			struct Up : public Base<Up>
			{
				Up(GridQuery<Up>& mQuery) : Base{mQuery}				{ }
				inline bool isValid()									{ return query.index.y >= query.grid.getIndexYMin(); }
				inline void step()										{ --query.index.y; }
				inline bool getSorting(const Body* mA, const Body* mB)	{ return mA->getPosition().y < mB->getPosition().y; }
				inline bool misses(const AABB& mShape)					{ return mShape.getTop() > query.pos.y || query.pos.x < mShape.getLeft() || query.pos.x > mShape.getRight(); }
				inline void setOut(const AABB& mShape)					{ query.lastPos = Vec2f(query.pos.x, mShape.getBottom()); }
			};
			struct Down : public Base<Down>
			{
				Down(GridQuery<Down>& mQuery) : Base{mQuery}			{ }
				inline bool isValid()									{ return query.index.y < query.grid.getIndexYMax(); }
				inline void step()										{ ++query.index.y; }
				inline bool getSorting(const Body* mA, const Body* mB)	{ return mA->getPosition().y > mB->getPosition().y; }
				inline bool misses(const AABB& mShape)					{ return mShape.getBottom() < query.pos.y || query.pos.x < mShape.getLeft() || query.pos.x > mShape.getRight(); }
				inline void setOut(const AABB& mShape)					{ query.lastPos = Vec2f(query.pos.x, mShape.getTop()); }
			};
		}

		struct Point : public Base<Point>
		{
			bool finished{false};
			Point(GridQuery<Point>& mQuery) : Base{mQuery}				{ }
			inline bool isValid()										{ return !finished && query.grid.isIndexValid(query.index); }
			inline void step()											{ finished = true; }
			inline bool getSorting(const Body*, const Body*)			{ return true; }
			inline bool misses(const AABB& mShape)						{ return !mShape.contains(Vec2i(query.pos)); }
			inline void setOut(const AABB&)								{ }
		};

		struct RayCast : public Base<RayCast>
		{
			int cellSize;
			Vec2i next{0, 0};
			Vec2f direction, deltaDist, increment, max;

			RayCast(GridQuery<RayCast>& mQuery, Vec2f mDirection);

			inline bool isValid() { return query.grid.isIndexValid(query.index); }
			void step();
			bool getSorting(const Body* mA, const Body* mB);
			bool misses(const AABB& mShape);
			void setOut(const AABB&) { }
		};

		struct Distance : public Base<Distance>
		{
			int cellSize, distance, cellRadius;
			std::queue<Vec2i> offsets;

			Distance(GridQuery<Distance>& mQuery, int mDistance);

			inline bool isValid() { return !offsets.empty() && query.grid.isIndexValid(query.index); }
			void step();
			bool getSorting(const Body* mA, const Body* mB);
			bool misses(const AABB& mShape);
			inline void setOut(const AABB&) { }
		};
	}
}

#endif
