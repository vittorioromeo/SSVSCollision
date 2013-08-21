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
#include "SSVSCollision/Utils/Utils.h"

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
				template<typename T> inline static void getBodies(std::vector<Body*>& mBodies, const T& mInternal) { mBodies = mInternal.grid.getCell(mInternal.index).getBodies(); }
			};
			struct ByGroup
			{
				template<typename T> inline static void getBodies(std::vector<Body*>& mBodies, const T& mInternal, Group mGroup)
				{
					std::vector<Body*> result;
					for(const auto& b : mInternal.grid.getCell(mInternal.index).getBodies()) if(b->hasGroup(mGroup)) result.push_back(b);
					mBodies = result;
				}
			};
		}

		struct OrthoLeft : public Base
		{
			template<typename... TArgs> OrthoLeft(TArgs&&... mArgs) : Base(std::forward<TArgs>(mArgs)...) { }
			inline bool isValid()									{ return index.x >= grid.getIndexXMin(); }
			inline void step()										{ --index.x; }
			inline bool getSorting(const Body* mA, const Body* mB)	{ return mA->getPosition().x < mB->getPosition().x; }
			inline bool hits(const AABB& mShape)					{ return mShape.getLeft() <= pos.x && pos.y >= mShape.getTop() && pos.y <= mShape.getBottom(); }
			inline void setOut(const AABB& mShape)					{ lastPos = Vec2f(mShape.getRight(), pos.y); }
		};
		struct OrthoRight : public Base
		{
			template<typename... TArgs> OrthoRight(TArgs&&... mArgs) : Base(std::forward<TArgs>(mArgs)...) { }
			inline bool isValid()									{ return index.x < grid.getIndexXMax(); }
			inline void step()										{ ++index.x; }
			inline bool getSorting(const Body* mA, const Body* mB)	{ return mA->getPosition().x > mB->getPosition().x; }
			inline bool hits(const AABB& mShape)					{ return mShape.getRight() >= pos.x && pos.y >= mShape.getTop() && pos.y <= mShape.getBottom(); }
			inline void setOut(const AABB& mShape)					{ lastPos = Vec2f(mShape.getLeft(), pos.y); }
		};
		struct OrthoUp : public Base
		{
			template<typename... TArgs> OrthoUp(TArgs&&... mArgs) : Base(std::forward<TArgs>(mArgs)...) { }
			inline bool isValid()									{ return index.y >= grid.getIndexYMin(); }
			inline void step()										{ --index.y; }
			inline bool getSorting(const Body* mA, const Body* mB)	{ return mA->getPosition().y < mB->getPosition().y; }
			inline bool hits(const AABB& mShape)					{ return mShape.getTop() <= pos.y && pos.x >= mShape.getLeft() && pos.x <= mShape.getRight(); }
			inline void setOut(const AABB& mShape)					{ lastPos = Vec2f(pos.x, mShape.getBottom()); }
		};
		struct OrthoDown : public Base
		{
			template<typename... TArgs> OrthoDown(TArgs&&... mArgs) : Base(std::forward<TArgs>(mArgs)...) { }
			inline bool isValid()									{ return index.y < grid.getIndexYMax(); }
			inline void step()										{ ++index.y; }
			inline bool getSorting(const Body* mA, const Body* mB)	{ return mA->getPosition().y > mB->getPosition().y; }
			inline bool hits(const AABB& mShape)					{ return mShape.getBottom() >= pos.y && pos.x >= mShape.getLeft() && pos.x <= mShape.getRight(); }
			inline void setOut(const AABB& mShape)					{ lastPos = Vec2f(pos.x, mShape.getTop()); }
		};

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
			Vec2f dir, deltaDist, increment, max;

			RayCast(Grid& mGrid, Vec2i mPos, Vec2f mDir) : Base{mGrid, mPos}, cellSize{grid.getCellSize()}, dir{ssvs::Utils::getNormalized(mDir)},
				deltaDist(cellSize / abs(dir.x), cellSize / abs(dir.y)), increment{dir * static_cast<float>(cellSize)}, max{Vec2f(startIndex * cellSize) - startPos}
			{
				next.x = dir.x < 0 ? -1 : 1;
				next.y = dir.y < 0 ? -1 : 1;
				if(dir.x >= 0) max.x += cellSize;
				if(dir.y >= 0) max.y += cellSize;
				max.x /= dir.x;
				max.y /= dir.y;
			}

			inline bool isValid() { return grid.isIndexValid(index); }
			inline void step()
			{
				lastPos = pos;
				pos += increment;

				if(max.x < max.y)	{ max.x += deltaDist.x; index.x += next.x; }
				else				{ max.y += deltaDist.y; index.y += next.y; }
			}
			inline bool getSorting(const Body* mA, const Body* mB)
			{
				const auto& aPos(mA->getPosition());
				const auto& bPos(mB->getPosition());
				return pow((aPos.x - startPos.x), 2) + pow((aPos.y - startPos.y), 2) > pow((bPos.x - startPos.x), 2) + pow((bPos.y - startPos.y), 2);
			}
			bool hits(const AABB& mShape)
			{
				Segment<float> ray{startPos, pos};
				Segment<float> test1{dir.x > 0 ? mShape.getSegmentLeft<float>() : mShape.getSegmentRight<float>()};
				Segment<float> test2{dir.y > 0 ? mShape.getSegmentTop<float>() : mShape.getSegmentBottom<float>()};

				Vec2f intersection;
				if(Utils::isSegmentInsersecting(ray, test1, intersection) || Utils::isSegmentInsersecting(ray, test2, intersection))
				{
					lastPos = intersection;
					return true;
				}

				return false;
			}
			inline void setOut(const AABB&) { }
		};



		struct Distance : public Base
		{
			int cellSize, distance, cellRadius;
			std::queue<Vec2i> offsets;

			Distance(Grid& mGrid, Vec2i mPos, int mDistance) : Base{mGrid, mPos}, cellSize{grid.getCellSize()}, distance{mDistance}, cellRadius{distance / cellSize}
			{
				for(int iRadius{0}; iRadius < cellRadius + 1; ++iRadius)
				{
					for(int iY{-iRadius}; iY <= iRadius; ++iY)
					{
						offsets.emplace(iRadius, iY);
						if(-iRadius != iRadius) offsets.emplace(-iRadius, iY);
					}
					for(int iX{-iRadius + 1}; iX <= iRadius -1; ++iX)
					{
						offsets.emplace(iX, iRadius);
						offsets.emplace(iX, -iRadius);
					}
				}
			}

			inline bool isValid() { return !offsets.empty() && grid.isIndexValid(index); }
			inline void step()
			{
				lastPos = pos;
				index = startIndex + offsets.front();
				if(!offsets.empty()) offsets.pop();
			}
			inline bool getSorting(const Body* mA, const Body* mB)
			{
				const auto& aPos(mA->getPosition());
				const auto& bPos(mB->getPosition());
				return pow((aPos.x - startPos.x), 2) + pow((aPos.y - startPos.y), 2) > pow((bPos.x - startPos.x), 2) + pow((bPos.y - startPos.y), 2);
			}
			bool hits(const AABB& mShape)
			{
				int testX{startPos.x < mShape.getX() ? mShape.getLeft() : mShape.getRight()};
				int testY{startPos.y < mShape.getY() ? mShape.getTop() : mShape.getBottom()};

				if(pow((testX - startPos.x), 2) + pow((testY - startPos.y), 2) > pow(distance, 2)) return false;

				lastPos = Vec2f(testX, testY);
				return true;
			}
			inline void setOut(const AABB&) { }
		};
	}
}

#endif
