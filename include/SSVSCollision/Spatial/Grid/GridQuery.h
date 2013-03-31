// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_GRIDQUERY
#define SSVSC_GRIDQUERY

#include <vector>
#include <SFML/System.hpp>
#include <SSVStart/SSVStart.h>
#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"
#include "SSVSCollision/Body/Body.h"

namespace ssvsc
{	
	sf::Vector2i lineIntersection(sf::Vector2i mA1, sf::Vector2i mA2, sf::Vector2i mY1, sf::Vector2i mY2);

	namespace QueryTraits
	{
		namespace Bodies
		{
			struct All
			{
				static void getBodies(Grid& mGrid, std::vector<Body*>& mBodies, const sf::Vector2i& mIndex, const std::string&) 
				{
					mBodies = mGrid.getCell(mIndex).getBodies(); 
				}
			};
			struct Grouped
			{
				static void getBodies(Grid& mGrid, std::vector<Body*>& mBodies, const sf::Vector2i& mIndex, const std::string& mGroup) 
				{ 
					mBodies = mGrid.getCell(mIndex).getBodies(mGroup); 
				}
			};	
		}
		
		namespace Orthogonal
		{
			struct Left
			{
				static bool isValid(const Grid& mGrid, const sf::Vector2i& mIndex) { return mIndex.x >= mGrid.getXMinIndex(); }
				static void step(sf::Vector2i& mIndex, sf::Vector2i&, sf::Vector2i&, sf::Vector2f&, const sf::Vector2i&, const sf::Vector2f&, const sf::Vector2f&, int)
				{ 
					--mIndex.x; 
				}
				static bool getSorting(const Body* mA, const Body* mB, const sf::Vector2i&) 
				{ 
					return mA->getPosition().x < mB->getPosition().x; 
				}
				static bool misses(const sf::Vector2i& mPos, const AABB& mShape, const sf::Vector2i&) 
				{ 
					return mShape.getLeft() > mPos.x || mPos.y < mShape.getTop() || mPos.y > mShape.getBottom(); 
				}
				static void setPos(sf::Vector2i& mPos, const AABB& mShape) { mPos.x = mShape.getRight(); } 
			};		
			struct Right
			{
				static bool isValid(const Grid& mGrid, const sf::Vector2i& mIndex) { return mIndex.x < mGrid.getXMaxIndex(); }
				static void step(sf::Vector2i& mIndex, sf::Vector2i&, sf::Vector2i&, sf::Vector2f&, const sf::Vector2i&, const sf::Vector2f&, const sf::Vector2f&, int)
				{ 
					++mIndex.x; 
				}
				static bool getSorting(const Body* mA, const Body* mB, const sf::Vector2i&) 
				{ 
					return mA->getPosition().x > mB->getPosition().x; 
				}
				static bool misses(const sf::Vector2i& mPos, const AABB& mShape, const sf::Vector2i&) 
				{ 
					return mShape.getRight() < mPos.x || mPos.y < mShape.getTop() || mPos.y > mShape.getBottom(); 
				}
				static void setPos(sf::Vector2i& mPos, const AABB& mShape) { mPos.x = mShape.getLeft(); } 
			};		
			struct Up
			{
				static bool isValid(const Grid& mGrid, const sf::Vector2i& mIndex) { return mIndex.y >= mGrid.getYMinIndex(); }
				static void step(sf::Vector2i& mIndex, sf::Vector2i&, sf::Vector2i&, sf::Vector2f&, const sf::Vector2i&, const sf::Vector2f&, const sf::Vector2f&, int)
				{ 
					--mIndex.y; 
				}
				static bool getSorting(const Body* mA, const Body* mB, const sf::Vector2i&) 
				{ 
					return mA->getPosition().y < mB->getPosition().y; 
				}
				static bool misses(const sf::Vector2i& mPos, const AABB& mShape, const sf::Vector2i&) 
				{ 
					return mShape.getTop() > mPos.y || mPos.x < mShape.getLeft() || mPos.x > mShape.getRight(); 
				}
				static void setPos(sf::Vector2i& mPos, const AABB& mShape) { mPos.y = mShape.getBottom(); } 
			};		
			struct Down
			{
				static bool isValid(const Grid& mGrid, const sf::Vector2i& mCurrentIndex) { return mCurrentIndex.y < mGrid.getYMaxIndex(); }
				static void step(sf::Vector2i& mIndex, sf::Vector2i&, sf::Vector2i&, sf::Vector2f&, const sf::Vector2i&, const sf::Vector2f&, const sf::Vector2f&, int)
				{ 
					++mIndex.y; 
				}
				static bool getSorting(const Body* mA, const Body* mB, const sf::Vector2i&) 
				{ 
					return mA->getPosition().y > mB->getPosition().y; 
				}
				static bool misses(const sf::Vector2i& mPos, const AABB& mShape, const sf::Vector2i&) 
				{ 
					return mShape.getBottom() < mPos.y || mPos.x < mShape.getLeft() || mPos.x > mShape.getRight(); 
				}
				static void setPos(sf::Vector2i& mPos, const AABB& mShape) { mPos.y = mShape.getTop(); } 
			};
		}
		
		struct RayCast
		{
			static bool isValid(const Grid& mGrid, const sf::Vector2i& mIndex) 
			{ 
				return mIndex.x >= mGrid.getXMinIndex() && mIndex.x < mGrid.getXMaxIndex() && mIndex.y >= mGrid.getYMinIndex() && mIndex.y < mGrid.getYMaxIndex(); 
			}
			static void step(sf::Vector2i& mIndex, sf::Vector2i& mPos, sf::Vector2i& mStep, sf::Vector2f& mSideDist, 
				const sf::Vector2i& mStartIndex, const sf::Vector2f& mDirection, const sf::Vector2f& mDeltaDist, int mCellSize)
			{ 
				mPos += sf::Vector2i(mDirection * static_cast<float>(mCellSize));
				
				if(mDirection.x < 0) { mStep.x = -1; mSideDist.x = (mStartIndex.x - mIndex.x) * mDeltaDist.x; }
				else { mStep.x = 1; mSideDist.x = (mIndex.x + 1.0f - mStartIndex.x) * mDeltaDist.x; }
				if(mDirection.y < 0) { mStep.y = -1; mSideDist.y = (mStartIndex.y - mIndex.y) * mDeltaDist.y; }
				else { mStep.y = 1; mSideDist.y = (mIndex.y + 1.0f - mStartIndex.y) * mDeltaDist.y; }
				
				if(mSideDist.x < mSideDist.y) { mSideDist.x += mDeltaDist.x; mIndex.x += mStep.x; }
				else { mSideDist.y += mSideDist.y; mIndex.y += mStep.y; }
			}
			static bool getSorting(const Body* mA, const Body* mB, const sf::Vector2i& mStartPos) 
			{ 
				return sqrt(pow((mA->getPosition().x - mStartPos.x), 2) + pow((mA->getPosition().y - mStartPos.y), 2)) > 
						sqrt(pow((mB->getPosition().x - mStartPos.x), 2) + pow((mB->getPosition().y - mStartPos.y), 2));
			}
			static bool misses(sf::Vector2i& mPos, const AABB& mShape, const sf::Vector2i& mStartPos) 
			{ 				
				std::vector<std::pair<sf::Vector2i, sf::Vector2i>> lines;
				lines.push_back({mShape.getNWCorner(), mShape.getNECorner()});
				lines.push_back({mShape.getNECorner(), mShape.getSECorner()});
				lines.push_back({mShape.getSECorner(), mShape.getSWCorner()});
				lines.push_back({mShape.getSWCorner(), mShape.getNWCorner()});
				
				sf::Vector2i intersection{0, 0};
				while(intersection.x == 0 && intersection.y == 0 && !lines.empty())
				{
					auto currentLine(lines.back());
					lines.pop_back();
					intersection = lineIntersection(mStartPos, mPos, currentLine.first, currentLine.second);
				}
					
				if(intersection.x == 0 && intersection.y == 0) return true;
				else
				{
					mPos = intersection;
					return false;	
				}
			}
			static void setPos(sf::Vector2i&, const AABB&) { } 
		};
	}
	
	class GridQuery
	{
		private:
			Grid& grid;
			std::vector<Body*> bodies;
			sf::Vector2i startPos, pos, startIndex, index, step;
			sf::Vector2f direction, sideDist, deltaDist{(float)sqrt(1 + (direction.y * direction.y) / (direction.x * direction.x)), (float)sqrt(1 + (direction.x * direction.x) / (direction.y * direction.y))};

			template<typename TQueryTraits, typename TCellTraits> Body* nextImpl(const std::string& mGroup = "")
			{
				while(TQueryTraits::isValid(grid, index))
				{				
					if(bodies.empty())
					{			
						TCellTraits::getBodies(grid, bodies, index, mGroup);
						TQueryTraits::step(index, pos, step, sideDist, startIndex, direction, deltaDist, grid.getCellSize());
						ssvu::sort(bodies, [&](const Body* mA, const Body* mB){ return TQueryTraits::getSorting(mA, mB, startPos); });
					}				
					
					while(!bodies.empty())
					{
						Body* body{bodies.back()};
						auto& shape(body->getShape());
						bodies.pop_back();
						
						if(TQueryTraits::misses(pos, shape, startPos)) continue;
						
						TQueryTraits::setPos(pos, shape);
						return body;
					}
				}
				
				return nullptr;
			}
		
		public:
			GridQuery(Grid& mGrid, sf::Vector2i mStartPos);
		
			template<typename TQueryTraits> Body* next() { return nextImpl<TQueryTraits, QueryTraits::Bodies::All>(); }
			template<typename TQueryTraits> Body* next(const std::string& mGroup) { return nextImpl<TQueryTraits, QueryTraits::Bodies::Grouped>(mGroup); }
			Body* nextByDirection(sf::Vector2f mDirection);
			Body* nextByDirection(sf::Vector2f mDirection, const std::string& mGroup);
			
			void reset();
						
			// Getters
			sf::Vector2i getStartPos();
			sf::Vector2i getStartIndex();
			sf::Vector2i getPos();
			sf::Vector2i getIndex();
	};
}

#endif
