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
			World* world;
			bool isStatic;
			std::unordered_set<Cell*> cells;
			sf::Vector2i position;
			sf::Vector2i previousPosition;
			sf::Vector2i velocity;
			sf::Vector2i halfSize;
			std::unordered_set<std::string> groups;
			std::unordered_set<std::string> groupsToCheck;
			std::unordered_set<std::string> groupsToIgnoreResolve;
			ssvs::Delegate<CollisionInfo> OnCollision;
			ssvs::Delegate<void> OnOutOfBounds;
			void* userData;
			
		public:
			Body(World* mWorld, bool mIsStatic, sf::Vector2i mPosition, int mWidth, int mHeight);
			~Body();

			// Group-related methods
			void addGroups(const std::vector<std::string>& mGroups);
			void addGroupsToCheck(const std::vector<std::string>& mGroups);
			void addGroupsToIgnoreResolve(const std::vector<std::string>& mGroups);

			bool isOverlapping(Body* mBody);
			void update(float mFrameTime);

			// Setters
			void setCells(std::unordered_set<Cell*> mCells);
			void setPosition(sf::Vector2i mPosition);
			void setVelocity(sf::Vector2i mVelocity);

			// Getters
			std::unordered_set<Cell*> getCells();
			std::unordered_set<std::string> getGroups();
			std::unordered_set<std::string> getGroupsToCheck();
			sf::Vector2i getPosition();
			sf::Vector2i getVelocity();
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
