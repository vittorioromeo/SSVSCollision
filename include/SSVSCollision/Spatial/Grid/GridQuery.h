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
				static void getBodies(Grid& mGrid, std::vector<Body*>& mBodies, const sf::Vector2i& mIndex, const std::string&) { mBodies = mGrid.getCell(mIndex).getBodies(); }
			};
			struct Grouped
			{
				static void getBodies(Grid& mGrid, std::vector<Body*>& mBodies, const sf::Vector2i& mIndex, const std::string& mGroup) { mBodies = mGrid.getCell(mIndex).getBodies(mGroup); }
			};	
		}
		
		namespace Orthogonal
		{
			struct Left
			{
				static bool isValid(const Grid& mGrid, const sf::Vector2i& mCurrentIndex) { return mCurrentIndex.x >= mGrid.getXMinIndex(); }
				static void increment(sf::Vector2i& mCurrentIndex) { --mCurrentIndex.x; }
				static bool getSorting(const Body* mA, const Body* mB) { return mA->getPosition().x < mB->getPosition().x; }
				static bool misses(const sf::Vector2i& mCurrent, const AABB& mShape) { return mShape.getLeft() > mCurrent.x || mCurrent.y < mShape.getTop() || mCurrent.y > mShape.getBottom(); }
				static void setCurrent(sf::Vector2i& mCurrent, const AABB& mShape) { mCurrent.x = mShape.getRight(); } 
			};		
			struct Right
			{
				static bool isValid(const Grid& mGrid, const sf::Vector2i& mCurrentIndex) { return mCurrentIndex.x < mGrid.getXMaxIndex(); }
				static void increment(sf::Vector2i& mCurrentIndex) { ++mCurrentIndex.x; }
				static bool getSorting(const Body* mA, const Body* mB) { return mA->getPosition().x > mB->getPosition().x; }
				static bool misses(const sf::Vector2i& mCurrent, const AABB& mShape) { return mShape.getRight() < mCurrent.x || mCurrent.y < mShape.getTop() || mCurrent.y > mShape.getBottom(); }
				static void setCurrent(sf::Vector2i& mCurrent, const AABB& mShape) { mCurrent.x = mShape.getLeft(); } 
			};		
			struct Up
			{
				static bool isValid(const Grid& mGrid, const sf::Vector2i& mCurrentIndex) { return mCurrentIndex.y >= mGrid.getYMinIndex(); }
				static void increment(sf::Vector2i& mCurrentIndex) { --mCurrentIndex.y; }
				static bool getSorting(const Body* mA, const Body* mB) { return mA->getPosition().y < mB->getPosition().y; }
				static bool misses(const sf::Vector2i& mCurrent, const AABB& mShape) { return mShape.getTop() > mCurrent.y || mCurrent.x < mShape.getLeft() || mCurrent.x > mShape.getRight(); }
				static void setCurrent(sf::Vector2i& mCurrent, const AABB& mShape) { mCurrent.y = mShape.getBottom(); } 
			};		
			struct Down
			{
				static bool isValid(const Grid& mGrid, const sf::Vector2i& mCurrentIndex) { return mCurrentIndex.y < mGrid.getYMaxIndex(); }
				static void increment(sf::Vector2i& mCurrentIndex) { ++mCurrentIndex.y; }
				static bool getSorting(const Body* mA, const Body* mB) { return mA->getPosition().y > mB->getPosition().y; }
				static bool misses(const sf::Vector2i& mCurrent, const AABB& mShape) { return mShape.getBottom() < mCurrent.y || mCurrent.x < mShape.getLeft() || mCurrent.x > mShape.getRight(); }
				static void setCurrent(sf::Vector2i& mCurrent, const AABB& mShape) { mCurrent.y = mShape.getTop(); } 
			};
		}
		
	}
	
	class GridQuery
	{
		private:
			Grid& grid;
			std::vector<Body*> currentBodies;
			sf::Vector2i start, current, startIndex, currentIndex, step;
			sf::Vector2f direction, sideDist, deltaDist{(float)sqrt(1 + (direction.y * direction.y) / (direction.x * direction.x)), (float)sqrt(1 + (direction.x * direction.x) / (direction.y * direction.y))};
						
			void steap();		
						
			template<typename TQueryTraits, typename TCellTraits> Body* nextImpl(const std::string& mGroup = "")
			{
				while(TQueryTraits::isValid(grid, currentIndex))
				{				
					if(currentBodies.empty())
					{
						TCellTraits::getBodies(grid, currentBodies, currentIndex, mGroup);
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
		
		public:
			GridQuery(Grid& mGrid, sf::Vector2i mStart);
		
			template<typename TQueryTraits> Body* next() { return nextImpl<TQueryTraits, QueryTraits::Bodies::All>(); }
			template<typename TQueryTraits> Body* next(const std::string& mGroup) { return nextImpl<TQueryTraits, QueryTraits::Bodies::Grouped>(mGroup); }
			
			void reset();
			
			Body* test(sf::Vector2f mDirection);
						
			// Getters
			sf::Vector2i getStart();
			sf::Vector2i getStartIndex();
			sf::Vector2i getCurrent();
			sf::Vector2i getCurrentIndex();
	};
}

#endif
