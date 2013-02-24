// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef BODY_H
#define BODY_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SSVStart.h>
#include "AABB/AABB.h"
#include "CallbackInfo.h"

namespace ssvsc
{
	class World;
	class ResolverBase;
	class SpatialInfoBase;

	class Body
	{
		private:
			World& world;
			ResolverBase& resolver;
			SpatialInfoBase& spatialInfo;
			AABB shape, oldShape;
			bool _static, outOfBounds{false};
			sf::Vector2f velocity, acceleration;
			std::vector<std::string> groups, groupsToCheck, groupsNoResolve;
			void* userData{nullptr};

			void integrate(float mFrameTime);
			
		public:
			ssvs::Delegate<void> onPreUpdate;
			ssvs::Delegate<void> onPostUpdate;
			ssvs::Delegate<void, DetectionInfo> onDetection;
			ssvs::Delegate<void, ResolutionInfo> onResolution;
			ssvs::Delegate<void> onOutOfBounds;

			Body(World& mWorld, bool mIsStatic, sf::Vector2i mPosition, sf::Vector2i mSize);
			~Body();

			void addGroups(const std::vector<std::string>& mGroups);
			void addGroupsToCheck(const std::vector<std::string>& mGroups);
			void addGroupsNoResolve(const std::vector<std::string>& mGroups);
			void update(float mFrameTime);
			void applyForce(sf::Vector2f mForce);
			void destroy();

			// Setters
			void setPosition(sf::Vector2i mPosition);
			void setVelocity(sf::Vector2f mVelocity);
			void setAcceleration(sf::Vector2f mAcceleration);
			void setStatic(bool mStatic);
			void setUserData(void* mUserData);
			void setX(int mX);
			void setY(int mY);
			void setVelocityX(float mX);
			void setVelocityY(float mY);
			void setSize(sf::Vector2i mSize);
			void setWidth(int mWidth);
			void setHeight(int mHeight);
			void setOutOfBounds(bool mOutOfBounds);

			// Getters
			AABB& getShape();
			AABB& getOldShape();
			const std::vector<std::string>& getGroups();
			const std::vector<std::string>& getGroupsToCheck();
			const std::vector<std::string>& getGroupsNoResolve();
			sf::Vector2i getPosition() const;
			sf::Vector2f getVelocity() const;
			sf::Vector2f getAcceleration() const;
			sf::Vector2i getSize() const;
			int getWidth() const;
			int getHeight() const;
			bool isStatic() const;
			void* getUserData() const;
			bool hasMovedLeft() const;
			bool hasMovedRight() const;
			bool hasMovedUp() const;
			bool hasMovedDown() const;
	};
}

#endif // BODY_H
