#ifndef BODY_H
#define BODY_H

#include "AABB/AABB.h"
#include "CollisionInfo.h"
#include "Grid/GridInfo.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SSVStart.h>

namespace ssvsc
{
	class World;
	class Grid;
	class Cell;

	class Body
	{
		private:
			World& world;
			Grid& grid;
			GridInfo gridInfo;
			AABB shape, oldShape;
			bool isStatic;
			sf::Vector2i velocity;
			std::vector<std::string> groups, groupsToCheck, groupsNoResolve;
			void* userData;

			void integrate(float mFrameTime);
			void resolve(const AABB& mShape);

		public:
			ssvs::Delegate<void, CollisionInfo> onCollision;
			ssvs::Delegate<void> onOutOfBounds;

			Body(World& mWorld, bool mIsStatic, sf::Vector2i mPosition, int mWidth, int mHeight);
			
			void addGroups(const std::vector<std::string>& mGroups);
			void addGroupsToCheck(const std::vector<std::string>& mGroups);
			void addGroupsNoResolve(const std::vector<std::string>& mGroups);
			void update(float mFrameTime);

			void setPosition(sf::Vector2i mPosition);
			void setVelocity(sf::Vector2i mVelocity);
			void setStatic(bool mStatic);
			void setUserData(void* mUserData);
			AABB& getShape();
			AABB& getOldShape();
			const std::vector<std::string>& getGroups();
			const std::vector<std::string>& getGroupsToCheck();
			sf::Vector2i getPosition();
			sf::Vector2i getVelocity();			
			bool getStatic();
			void* getUserData();
	};
}

#endif // BODY_H
