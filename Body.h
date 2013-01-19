#ifndef BODY_H
#define BODY_H

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

			std::vector<std::vector<Body*>*> queries; // Cell vector ptrs to query
			int startX, startY, endX, endY; // Edge cell positions
			bool mustRecalculate{false}; // IF TRUE CRASHES ON START - MUST FIX

			void recalcEdges(); // Sets startX, startY, endX, endY
			void checkOldEdges(); // Checks if startXY... is different from previousStartXY... - if so, recalculates
			void recalcCells(); // Clears cells/queries and gets new ones
			void resolve(Body* mBody);

		public:
			Body(World& mWorld, bool mIsStatic, sf::Vector2i mPosition, int mWidth, int mHeight);

			void clearCells();

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
			const std::vector<std::string>& getGroupsToCheck();
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
