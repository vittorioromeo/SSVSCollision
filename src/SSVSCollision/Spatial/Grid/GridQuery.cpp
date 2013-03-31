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
	Vector2i lineIntersection(Vector2i mA1, Vector2i mA2, Vector2i mB1, Vector2i mB2)
	{
		float d = (mA1.x - mA2.x) * (mB1.y - mB2.y) - (mA1.y - mA2.y) * (mB1.x - mB2.x);
		// If d is zero, there is no intersection
		if (d == 0) return Vector2i(0, 0);
		 
		// Get the x and y
		float pre = (mA1.x*mA2.y - mA1.y*mA2.x), post = (mB1.x*mB2.y - mB1.y*mB2.x);
		float x = ( pre * (mB1.x - mB2.x) - (mA1.x - mA2.x) * post ) / d;
		float y = ( pre * (mB1.y - mB2.y) - (mA1.y - mA2.y) * post ) / d;
		 
		// Check if the x and y coordinates are within both lines
		if ( x < min(mA1.x, mA2.x) || x > max(mA1.x, mA2.x) ||
		x < min(mB1.x, mB2.x) || x > max(mB1.x, mB2.x) ) return Vector2i(0, 0);
		if ( y < min(mA1.y, mA2.y) || y > max(mA1.y, mA2.y) ||
		y < min(mB1.y, mB2.y) || y > max(mB1.y, mB2.y) ) return Vector2i(0, 0);
		 
		return Vector2i(x, y);
	}
	
	GridQuery::GridQuery(Grid& mGrid, Vector2i mStart) : grid(mGrid), start{mStart}, current{mStart}, 
		startIndex{grid.getIndex(start)}, currentIndex{grid.getIndex(current)}, direction(Vector2f(5.f, -2.f)) { }
	
	void GridQuery::reset()
	{
		current = start;
		currentIndex = {grid.getIndex(current)};
		deltaDist = {(float)sqrt(1 + (direction.y * direction.y) / (direction.x * direction.x)), (float)sqrt(1 + (direction.x * direction.x) / (direction.y * direction.y))};
		currentBodies.clear();
	}

	Body* GridQuery::test(sf::Vector2f mDirection)
	{		
		//auto normalized = ssvs::Utils::getNormalized(mDirection);
		//r.direction = normalized;
		//r = RayCaster(grid, Vector2f(start), Vector2f(-1.f, -2.f));
		
		while(currentIndex.x >= grid.getXMinIndex() && currentIndex.x < grid.getXMaxIndex() && currentIndex.y >= grid.getYMinIndex() && currentIndex.y < grid.getYMaxIndex())
		{	
			if(currentBodies.empty())
			{
				log("index: " + toStr(currentIndex.x) + " " + toStr(currentIndex.y));
				if(!(currentIndex.x >= grid.getXMinIndex() && currentIndex.x < grid.getXMaxIndex() && currentIndex.y >= grid.getYMinIndex() && currentIndex.y < grid.getYMaxIndex())) 
				{
					log("FUAK!!!");
					break;
				}
				
				currentBodies = grid.getCell(currentIndex).getBodies();
								
				steap();
					
				//if(currentBodies.size() > 0) log("n bodies: " + toStr(currentBodies.size()));
				
				ssvu::sort(currentBodies, [&](Body* mA, Body* mB)
				{  
					return sqrt(pow((mA->getPosition().x - start.x), 2) + pow((mA->getPosition().y - start.y), 2)) > 
							sqrt(pow((mB->getPosition().x - start.x), 2) + pow((mB->getPosition().y - start.y), 2));
				});
			}				
			
			while(!currentBodies.empty())
			{
				Body* body{currentBodies.back()};
				auto& shape(body->getShape());
				currentBodies.pop_back();
				
				Vector2i s = start;
				Vector2i e = Vector2i(current);
				
				vector<pair<Vector2i, Vector2i>> lines;
				lines.push_back({shape.getNWCorner(), shape.getNECorner()});
				lines.push_back({shape.getNECorner(), shape.getSECorner()});
				lines.push_back({shape.getSECorner(), shape.getSWCorner()});
				lines.push_back({shape.getSWCorner(), shape.getNWCorner()});
				
				Vector2i intersection{0, 0};
				while(intersection.x == 0 && intersection.y == 0 && !lines.empty())
				{
					pair<Vector2i, Vector2i> currentLine{lines.back()};
					lines.pop_back();
					intersection = lineIntersection(s, e, currentLine.first, currentLine.second);
					//log(toStr(intersection.x) + " " + toStr(intersection.y));
				}
					
				if(intersection.x == 0 && intersection.y == 0) continue;
				
				current = intersection;
				
				//log("body found");
				return body;
			}
		}
		
		return nullptr;
	}
	// Getters
	Vector2i GridQuery::getStart()			{ return start; }
	Vector2i GridQuery::getStartIndex()		{ return startIndex; }	
	Vector2i GridQuery::getCurrent()		{ return current; }
	Vector2i GridQuery::getCurrentIndex()	{ return currentIndex; }
	
	//5RayCaster::RayCaster(Grid& mGrid, Vector2f mStartPos, Vector2f mDirection) : rayPos{mStartPos}, direction{mDirection}, startIndex{mGrid.getIndex(rayPos.x), mGrid.getIndex(rayPos.y)} { } 
	
	void GridQuery::steap()
	{
		current += Vector2i(direction * 1600.f);
		
		if(direction.x < 0) { step.x = -1; sideDist.x = (startIndex.x - currentIndex.x) * deltaDist.x; }
		else { step.x = 1; sideDist.x = (currentIndex.x + 1.0f - startIndex.x) * deltaDist.x; }
		if(direction.y < 0) { step.y = -1; sideDist.y = (startIndex.y - currentIndex.y) * deltaDist.y; }
		else { step.y = 1; sideDist.y = (currentIndex.y + 1.0f - startIndex.y) * deltaDist.y; }
		
		if(sideDist.x < sideDist.y) { sideDist.x += deltaDist.x; currentIndex.x += step.x; }
		else { sideDist.y += deltaDist.y; currentIndex.y += step.y; }
			
		//log("raypos: " + toStr(rayPos.x) + " " + toStr(rayPos.y));
		//log("direction: " + toStr(direction.x) + " " + toStr(direction.y));
		//log("step: " + toStr(step.x) + " " + toStr(step.y));
		//log("sideDist: " + toStr(sideDist.x) + " " + toStr(sideDist.y));
	}
	
}
