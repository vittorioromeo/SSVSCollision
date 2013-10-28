// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRIDQUERYTYPES
#define SSVSC_SPATIAL_GRIDQUERYTYPES

#include <vector>
#include <queue>
#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Utils/Utils.h"

namespace ssvsc
{
	namespace GridQueryTypes
	{
		template<typename TW, typename TGrid> struct Base
		{
			TGrid& grid;
			Vec2f startPos, pos, lastPos;
			Vec2i startIndex, index;

			Base(TGrid& mGrid, const Vec2i& mPos) : grid(mGrid), startPos{mPos}, pos{mPos}, startIndex{grid.getIdx(Vec2i(mPos))}, index{startIndex} { }

			inline void reset() noexcept { pos = startPos; index = startIndex; }
			inline const Vec2f& getLastPos() const noexcept	{ return lastPos; }
		};

		namespace Bodies
		{
			template<typename TW> struct All
			{
				template<typename T> inline static void getBodies(std::vector<Body<TW>*>& mBodies, const T& mInternal) { mBodies = mInternal.grid.getCell(mInternal.index).getBodies(); }
			};
			template<typename TW> struct ByGroup
			{
				template<typename T> inline static void getBodies(std::vector<Body<TW>*>& mBodies, const T& mInternal, Group mGroup)
				{
					std::vector<Body<TW>*> result;
					for(const auto& b : mInternal.grid.getCell(mInternal.index).getBodies()) if(b->hasGroup(mGroup)) result.push_back(b);
					mBodies = result;
				}
			};
		}

		template<typename TW, typename TGrid> struct OrthoLeft : public Base<TW, TGrid>
		{
			template<typename... TArgs> OrthoLeft(TArgs&&... mArgs) : Base<TW, TGrid>(std::forward<TArgs>(mArgs)...) { }
			inline bool isValid()									{ return this->index.x >= this->grid.getIdxXMin(); }
			inline void step()										{ --this->index.x; }
			inline bool getSorting(const Body<TW>* mA, const Body<TW>* mB)	{ return mA->getPosition().x < mB->getPosition().x; }
			inline bool hits(const AABB& mShape)					{ return mShape.getLeft() <= this->pos.x && this->pos.y >= mShape.getTop() && this->pos.y <= mShape.getBottom(); }
			inline void setOut(const AABB& mShape)					{ this->lastPos = Vec2f(mShape.getRight(), this->pos.y); }
		};
		template<typename TW, typename TGrid> struct OrthoRight : public Base<TW, TGrid>
		{
			template<typename... TArgs> OrthoRight(TArgs&&... mArgs) : Base<TW, TGrid>(std::forward<TArgs>(mArgs)...) { }
			inline bool isValid()									{ return this->index.x < this->grid.getIdxXMax(); }
			inline void step()										{ ++this->index.x; }
			inline bool getSorting(const Body<TW>* mA, const Body<TW>* mB)	{ return mA->getPosition().x > mB->getPosition().x; }
			inline bool hits(const AABB& mShape)					{ return mShape.getRight() >= this->pos.x && this->pos.y >= mShape.getTop() && this->pos.y <= mShape.getBottom(); }
			inline void setOut(const AABB& mShape)					{ this->lastPos = Vec2f(mShape.getLeft(), this->pos.y); }
		};
		template<typename TW, typename TGrid> struct OrthoUp : public Base<TW, TGrid>
		{
			template<typename... TArgs> OrthoUp(TArgs&&... mArgs) : Base<TW, TGrid>(std::forward<TArgs>(mArgs)...) { }
			inline bool isValid()									{ return this->index.y >= this->grid.getIdxYMin(); }
			inline void step()										{ --this->index.y; }
			inline bool getSorting(const Body<TW>* mA, const Body<TW>* mB)	{ return mA->getPosition().y < mB->getPosition().y; }
			inline bool hits(const AABB& mShape)					{ return mShape.getTop() <= this->pos.y && this->pos.x >= mShape.getLeft() && this->pos.x <= mShape.getRight(); }
			inline void setOut(const AABB& mShape)					{ this->lastPos = Vec2f(this->pos.x, mShape.getBottom()); }
		};
		template<typename TW, typename TGrid> struct OrthoDown : public Base<TW, TGrid>
		{
			template<typename... TArgs> OrthoDown(TArgs&&... mArgs) : Base<TW, TGrid>(std::forward<TArgs>(mArgs)...) { }
			inline bool isValid()									{ return this->index.y < this->grid.getIdxYMax(); }
			inline void step()										{ ++this->index.y; }
			inline bool getSorting(const Body<TW>* mA, const Body<TW>* mB)	{ return mA->getPosition().y > mB->getPosition().y; }
			inline bool hits(const AABB& mShape)					{ return mShape.getBottom() >= this->pos.y && this->pos.x >= mShape.getLeft() && this->pos.x <= mShape.getRight(); }
			inline void setOut(const AABB& mShape)					{ this->lastPos = Vec2f(this->pos.x, mShape.getTop()); }
		};

		template<typename TW, typename TGrid> struct Point : public Base<TW, TGrid>
		{
			bool finished{false};

			template<typename... TArgs> Point(TArgs&&... mArgs) : Base<TW, TGrid>(std::forward<TArgs>(mArgs)...) { }

			inline bool isValid()										{ return !finished && this->grid.isIdxValid(this->index); }
			inline void step()											{ finished = true; }
			inline bool getSorting(const Body<TW>*, const Body<TW>*)	{ return true; }
			inline bool hits(const AABB& mShape)						{ return mShape.contains(Vec2i(this->pos)); }
			inline void setOut(const AABB&)								{ }
		};

		template<typename TW, typename TGrid> struct RayCast : public Base<TW, TGrid>
		{
			int cellSize;
			Vec2i next;
			Vec2f dir, deltaDist, increment, max;

			RayCast(TGrid& mGrid, const Vec2i& mPos, const Vec2f& mDir) : Base<TW, TGrid>{mGrid, mPos}, cellSize{this->grid.getCellSize()}, dir{ssvs::getNormalized(mDir)},
				increment{dir * static_cast<float>(cellSize)}, max{Vec2f(this->startIndex * cellSize) - this->startPos}
			{
				next.x = dir.x < 0 ? -1 : 1;
				next.y = dir.y < 0 ? -1 : 1;
				if(dir.x >= 0) max.x += cellSize;
				if(dir.y >= 0) max.y += cellSize;

				if(dir.x != 0)
				{
					max.x /= dir.x;
					deltaDist.x = cellSize / std::abs(dir.x);
				}
				else
				{
					next.x = 0;
					deltaDist.x = cellSize;
				}

				if(dir.y != 0)
				{
					max.y /= dir.y;
					deltaDist.y = cellSize / std::abs(dir.y);
				}
				else
				{
					next.y = 0;
					deltaDist.y = cellSize;
				}
			}

			inline bool isValid() { return this->grid.isIdxValid(this->index); }
			inline void step()
			{
				this->lastPos = this->pos;
				this->pos += increment;

				if(max.x < max.y)	{ max.x += deltaDist.x; this->index.x += next.x; }
				else				{ max.y += deltaDist.y; this->index.y += next.y; }
			}
			inline bool getSorting(const Body<TW>* mA, const Body<TW>* mB)
			{
				return ssvs::getDistEuclidean(mA->getPosition(), this->startPos) > ssvs::getDistEuclidean(mB->getPosition(), this->startPos);
			}
			inline bool hits(const AABB& mShape)
			{
				Segment<float> ray{this->startPos, this->pos};
				Vec2f intersection;

				if(Utils::isSegmentInsersecting(ray, {dir.x > 0 ? mShape.getSegmentLeft<float>() : mShape.getSegmentRight<float>()}, intersection) ||
				   Utils::isSegmentInsersecting(ray, {dir.y > 0 ? mShape.getSegmentTop<float>() : mShape.getSegmentBottom<float>()}, intersection))
				{
					this->lastPos = intersection;
					return true;
				}

				return false;
			}
			inline void setOut(const AABB&) { }
		};



		template<typename TW, typename TGrid> struct Distance : public Base<TW, TGrid>
		{
			int cellSize, distance, cellRadius;
			std::queue<Vec2i> offsets;

			Distance(TGrid& mGrid, const Vec2i& mPos, int mDistance) : Base<TW, TGrid>{mGrid, mPos}, cellSize{this->grid.getCellSize()}, distance{mDistance}, cellRadius{distance / cellSize}
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

			inline bool isValid() { return !offsets.empty() && this->grid.isIdxValid(this->index); }
			inline void step()
			{
				this->lastPos = this->pos;
				this->index = this->startIndex + offsets.front();
				if(!offsets.empty()) offsets.pop();
			}
			inline bool getSorting(const Body<TW>* mA, const Body<TW>* mB)
			{
				return ssvs::getDistEuclidean(mA->getPosition(), this->startPos) > ssvs::getDistEuclidean(mB->getPosition(), this->startPos);
			}
			inline bool hits(const AABB& mShape)
			{
				Vec2i test{this->startPos.x < mShape.getX() ? mShape.getLeft() : mShape.getRight(), this->startPos.y < mShape.getY() ? mShape.getTop() : mShape.getBottom()};

				if(ssvs::getDistSquaredEuclidean(test, this->startPos) > pow(distance, 2)) return false;

				this->lastPos = Vec2f(test);
				return true;
			}
			inline void setOut(const AABB&) { }
		};
	}
}

#endif
