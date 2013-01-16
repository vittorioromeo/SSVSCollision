#ifndef BODY_H
#define BODY_H

#include <unordered_set>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SSVStart.h>
#include "CollisionInfo.h"

namespace ssvsc
{
	class World;
	class Cell;

	class Body
	{
		private:
			World& world;
			bool isStatic;
			std::vector<Cell*> cells;
			sf::Vector2i position, previousPosition, velocity, halfSize;
			std::vector<std::string> groups, groupsToCheck, groupsNoResolve;
			void* userData;

			int previousStartX, previousStartY, previousEndX, previousEndY;
			int startX, startY, endX, endY;			
			void checkCells();
			void recalculateCells();

		public:
			Body(World& mWorld, bool mIsStatic, sf::Vector2i mPosition, int mWidth, int mHeight);

			

			// Callback delegates
			ssvs::Delegate<void, CollisionInfo> onCollision;
			ssvs::Delegate<void> onOutOfBounds;

			// Group-related methods
			void addGroups(const std::vector<std::string>& mGroups);
			void addGroupsToCheck(const std::vector<std::string>& mGroups);
			void addGroupsNoResolve(const std::vector<std::string>& mGroups);

			bool isOverlapping(Body* mBody);
			void update(float mFrameTime);
			std::vector<Body*> getBodiesToCheck();

			// Properties
			void setPosition(sf::Vector2i mPosition);
			void setVelocity(sf::Vector2i mVelocity);
			const std::vector<std::string>& getGroups();
			sf::Vector2i getPosition();
			sf::Vector2i getVelocity();
			void* getUserData();
			int getX();
			int getY();
			int getLeft();
			int getRight();
			int getTop();
			int getBottom();
			int getHalfWidth();
			int getHalfHeight();
			int getWidth();
			int getHeight();
	};
}

#endif // BODY_H
