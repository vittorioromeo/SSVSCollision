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
		struct Base
		{
			Grid& grid;
			Vec2f startPos, pos, lastPos;
			Vec2i startIndex, index;

			Base(Grid& mGrid, Vec2i mPos) : grid(mGrid), startPos{mPos}, pos{mPos}, startIndex{grid.getIndex(Vec2i(mPos))}, index{startIndex} { }

			inline void reset()					{ pos = startPos; index = startIndex; }
			inline const Vec2f& getLastPos()	{ return lastPos; }
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
			struct Left : public Base
			{
				inline bool isValid()									{ return index.x >= grid.getIndexXMin(); }
				inline void step()										{ --index.x; }
				inline bool getSorting(const Body* mA, const Body* mB)	{ return mA->getPosition().x < mB->getPosition().x; }
				inline bool hits(const AABB& mShape)					{ return mShape.getLeft() <= pos.x && pos.y >= mShape.getTop() && pos.y <= mShape.getBottom(); }
				inline void setOut(const AABB& mShape)					{ lastPos = Vec2f(mShape.getRight(), pos.y); }
			};
			struct Right : public Base
			{
				inline bool isValid()									{ return index.x < grid.getIndexXMax(); }
				inline void step()										{ ++index.x; }
				inline bool getSorting(const Body* mA, const Body* mB)	{ return mA->getPosition().x > mB->getPosition().x; }
				inline bool hits(const AABB& mShape)					{ return mShape.getRight() >= pos.x && pos.y >= mShape.getTop() && pos.y <= mShape.getBottom(); }
				inline void setOut(const AABB& mShape)					{ lastPos = Vec2f(mShape.getLeft(), pos.y); }
			};
			struct Up : public Base
			{
				inline bool isValid()									{ return index.y >= grid.getIndexYMin(); }
				inline void step()										{ --index.y; }
				inline bool getSorting(const Body* mA, const Body* mB)	{ return mA->getPosition().y < mB->getPosition().y; }
				inline bool hits(const AABB& mShape)					{ return mShape.getTop() <= pos.y && pos.x >= mShape.getLeft() && pos.x <= mShape.getRight(); }
				inline void setOut(const AABB& mShape)					{ lastPos = Vec2f(pos.x, mShape.getBottom()); }
			};
			struct Down : public Base
			{
				inline bool isValid()									{ return index.y < grid.getIndexYMax(); }
				inline void step()										{ ++index.y; }
				inline bool getSorting(const Body* mA, const Body* mB)	{ return mA->getPosition().y > mB->getPosition().y; }
				inline bool hits(const AABB& mShape)					{ return mShape.getBottom() >= pos.y && pos.x >= mShape.getLeft() && pos.x <= mShape.getRight(); }
				inline void setOut(const AABB& mShape)					{ lastPos = Vec2f(pos.x, mShape.getTop()); }
			};
		}

		struct Point : public Base
		{
			bool finished{false};

			template<typename... TArgs> Point(TArgs&&... mArgs) : Base(std::forward<TArgs>(mArgs)...) { }

			inline bool isValid()										{ return !finished && grid.isIndexValid(index); }
			inline void step()											{ finished = true; }
			inline bool getSorting(const Body*, const Body*)			{ return true; }
			inline bool hits(const AABB& mShape)						{ return mShape.contains(Vec2i(pos)); }
			inline void setOut(const AABB&)								{ }
		};

		struct RayCast : public Base
		{
			int cellSize;
			Vec2i next{0, 0};
			Vec2f direction, deltaDist, increment, max;

			RayCast(Grid& mGrid, Vec2i mPos, Vec2f mDirection);

			inline bool isValid() { return grid.isIndexValid(index); }
			void step();
			bool getSorting(const Body* mA, const Body* mB);
			bool hits(const AABB& mShape);
			void setOut(const AABB&) { }
		};

		struct Distance : public Base
		{
			int cellSize, distance, cellRadius;
			std::queue<Vec2i> offsets;

			Distance(Grid& mGrid, Vec2i mPos, int mDistance);

			inline bool isValid() { return !offsets.empty() && grid.isIndexValid(index); }
			void step();
			bool getSorting(const Body* mA, const Body* mB);
			bool hits(const AABB& mShape);
			inline void setOut(const AABB&) { }
		};
	}
}

#endif
