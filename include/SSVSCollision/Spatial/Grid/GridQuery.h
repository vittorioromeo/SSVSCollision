// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_GRIDQUERY
#define SSVSC_GRIDQUERY

#include <vector>
#include <SFML/System.hpp>
#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"
#include "SSVSCollision/Body/Body.h"

namespace ssvsc
{	
	class GridQuery
	{
		public:
			struct TowardsLeft
			{
				static bool isValid(const Grid& mGrid, const sf::Vector2i& mCurrentIndex) { return mCurrentIndex.x >= mGrid.getXMinIndex(); }
				static void increment(sf::Vector2i& mCurrentIndex) { --mCurrentIndex.x; }
				static bool getSorting(const Body* mA, const Body* mB) { return mA->getPosition().x < mB->getPosition().x; }
				static bool misses(const sf::Vector2i& mCurrent, const AABB& mShape) { return mShape.getLeft() > mCurrent.x || mCurrent.y < mShape.getTop() || mCurrent.y > mShape.getBottom(); }
				static void setCurrent(sf::Vector2i& mCurrent, const AABB& mShape) { mCurrent.x = mShape.getRight(); } 
			};
			
			struct TowardsRight
			{
				static bool isValid(const Grid& mGrid, const sf::Vector2i& mCurrentIndex) { return mCurrentIndex.x < mGrid.getXMaxIndex(); }
				static void increment(sf::Vector2i& mCurrentIndex) { ++mCurrentIndex.x; }
				static bool getSorting(const Body* mA, const Body* mB) { return mA->getPosition().x > mB->getPosition().x; }
				static bool misses(const sf::Vector2i& mCurrent, const AABB& mShape) { return mShape.getRight() < mCurrent.x || mCurrent.y < mShape.getTop() || mCurrent.y > mShape.getBottom(); }
				static void setCurrent(sf::Vector2i& mCurrent, const AABB& mShape) { mCurrent.x = mShape.getLeft(); } 
			};
			
			struct TowardsTop
			{
				static bool isValid(const Grid& mGrid, const sf::Vector2i& mCurrentIndex) { return mCurrentIndex.y >= mGrid.getYMinIndex(); }
				static void increment(sf::Vector2i& mCurrentIndex) { --mCurrentIndex.y; }
				static bool getSorting(const Body* mA, const Body* mB) { return mA->getPosition().y < mB->getPosition().y; }
				static bool misses(const sf::Vector2i& mCurrent, const AABB& mShape) { return mShape.getTop() > mCurrent.y || mCurrent.x < mShape.getLeft() || mCurrent.x > mShape.getRight(); }
				static void setCurrent(sf::Vector2i& mCurrent, const AABB& mShape) { mCurrent.y = mShape.getBottom(); } 
			};
			
			struct TowardsBottom
			{
				static bool isValid(const Grid& mGrid, const sf::Vector2i& mCurrentIndex) { return mCurrentIndex.y < mGrid.getYMaxIndex(); }
				static void increment(sf::Vector2i& mCurrentIndex) { ++mCurrentIndex.y; }
				static bool getSorting(const Body* mA, const Body* mB) { return mA->getPosition().y > mB->getPosition().y; }
				static bool misses(const sf::Vector2i& mCurrent, const AABB& mShape) { return mShape.getBottom() < mCurrent.y || mCurrent.x < mShape.getLeft() || mCurrent.x > mShape.getRight(); }
				static void setCurrent(sf::Vector2i& mCurrent, const AABB& mShape) { mCurrent.y = mShape.getTop(); } 
			};
			
		private:
			Grid& grid;
			sf::Vector2i start, current, startIndex, currentIndex;
			std::vector<Body*> currentBodies;
		
		public:
			GridQuery(Grid& mGrid, sf::Vector2i mStart);
		
			template<typename TQueryTraits> Body* next()
			{
				while(TQueryTraits::isValid(grid, currentIndex))
				{				
					if(currentBodies.empty())
					{
						currentBodies = grid.getCell(currentIndex.x, currentIndex.y)->getBodies();
						TQueryTraits::increment(currentIndex);
						ssvu::sort(currentBodies, [&](const Body* mA, const Body* mB){ return TQueryTraits::getSorting(mA, mB); });
					}				
					
					while(!currentBodies.empty())
					{
						Body* body{currentBodies.back()};
						auto& shape(body->getShape());
			
						currentBodies.pop_back();
						
						if(TQueryTraits::misses(current, shape)) continue;
						
						TQueryTraits::setCurrent(current, shape);
						return body;
					}
				}
				
				return nullptr;
			}
			void reset();
						
			// Getters
			sf::Vector2i getStart();
			sf::Vector2i getStartIndex();
			sf::Vector2i getCurrent();
			sf::Vector2i getCurrentIndex();
	};
}

#endif
