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
			sf::Vector2f startPos, pos, direction, sideDist, deltaDist, out;

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
		
		public:
			GridQuery(Grid& mGrid, sf::Vector2i mStartPos);
		
			template<typename TQueryTraits> Body* next() { return nextImpl<TQueryTraits, QueryTraits::Bodies::All>(); }
			template<typename TQueryTraits> Body* next(const std::string& mGroup) { return nextImpl<TQueryTraits, QueryTraits::Bodies::Grouped>(mGroup); }
			Body* nextByDirection(sf::Vector2f mDirection);
			Body* nextByDirection(sf::Vector2f mDirection, const std::string& mGroup);
			
			void reset();
						
			// Getters
			Grid& getGrid() const;
			sf::Vector2f getStartPos() const;
			sf::Vector2i getStartIndex() const;
			sf::Vector2f getPos() const;
			sf::Vector2f getOut() const;
			sf::Vector2i getIndex() const;
			sf::Vector2f getDirection() const;
			sf::Vector2i getStep() const;
			sf::Vector2f getDeltaDist() const;
			sf::Vector2f getSideDist() const;
			
			// Setters
			void setBodies(std::vector<Body*> mBodies);
			void setPos(sf::Vector2f mPos);
			void setIndexX(int mIndexX);
			void setIndexY(int mIndexY);
			void setOutX(float mOutX);
			void setOutY(float mOutY);
			void setStepX(int mStepX);
			void setStepY(int mStepX);
			void setSideDistX(float mSideDistX);
			void setSideDistY(float mSideDistY);
			void setDirection(sf::Vector2f mDirection);
	};
}

#endif
