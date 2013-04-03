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
#include "SSVSCollision/Spatial/Grid/GridQueryTraits.h"

namespace ssvsc
{	
	class GridQuery
	{
		private:
			Grid& grid;
			std::vector<Body*> bodies;
			sf::Vector2i startIndex, index, step;
			sf::Vector2f startPos, pos, direction, deltaDist, out,  max, increment;
			std::vector<sf::Vector2i> visitedIndexes;

			template<typename TQueryTraits, typename TCellTraits> Body* nextImpl(const std::string& mGroup = "")
			{
				while(TQueryTraits::isValid(*this))
				{				
					if(bodies.empty())
					{
						TCellTraits::getBodies(*this, mGroup);
						TQueryTraits::step(*this);					
						ssvu::sort(bodies, [&](const Body* mA, const Body* mB){ return TQueryTraits::getSorting(*this, mA, mB); });
					}
					
					while(!bodies.empty())
					{
						Body* body{bodies.back()};
						auto& shape(body->getShape());
						bodies.pop_back();
						
						if(TQueryTraits::misses(*this, shape)) continue;
						
						TQueryTraits::setOut(*this, shape);
						return body;
					}
				}
				
				return nullptr;
			}
			
			template<typename TCellTraits> Body* nextHelper(sf::Vector2f mDirection, const std::string& mGroup = "")
			{
				setDirection(mDirection);
				if(mDirection.x == 0.f)
				{
					if(mDirection.y == 1.f)	return nextImpl<QueryTraits::Orthogonal::Down, TCellTraits>(mGroup);
					else if(mDirection.y == -1.f) return nextImpl<QueryTraits::Orthogonal::Up, TCellTraits>(mGroup);
				}
				else if(mDirection.y == 0.f)
				{
					if(mDirection.x == 1.f)	return nextImpl<QueryTraits::Orthogonal::Right, TCellTraits>(mGroup);
					else if(mDirection.x == -1.f) return nextImpl<QueryTraits::Orthogonal::Left, TCellTraits>(mGroup);
				}
				return nextImpl<QueryTraits::RayCast, TCellTraits>(mGroup);
			}
		
		public:
			GridQuery(Grid& mGrid, sf::Vector2i mStartPos);
		
			Body* next(sf::Vector2f mDirection);
			Body* next(sf::Vector2f mDirection, const std::string& mGroup);
			
			void reset();
						
			// Getters
			Grid& getGrid();
			const sf::Vector2f& getStartPos();
			const sf::Vector2i& getStartIndex();
			const sf::Vector2f& getPos();
			const sf::Vector2f& getOut();
			const sf::Vector2i& getIndex();
			const sf::Vector2f& getDirection();
			const sf::Vector2i& getStep();
			const sf::Vector2f& getDeltaDist();
			const std::vector<sf::Vector2i>& getVisitedIndexes(); // TODO: cannot the indexes be returned while querying?
			const sf::Vector2f& getMax();
			const sf::Vector2f& getIncrement();
			
			// Setters
			void setBodies(std::vector<Body*> mBodies);
			void setPos(sf::Vector2f mPos);
			void setIndexX(int mIndexX);
			void setIndexY(int mIndexY);
			void setOut(sf::Vector2f mOut);
			void setOutX(float mOutX);
			void setOutY(float mOutY);
			void setStepX(int mStepX);
			void setStepY(int mStepY);
			void setMaxX(float mMaxX);
			void setMaxY(float mMaxY);
			void setDirection(sf::Vector2f mDirection);
	};
	
	// TODO: consider removing everything but raycasting and forget about QueryTraits
}

#endif
