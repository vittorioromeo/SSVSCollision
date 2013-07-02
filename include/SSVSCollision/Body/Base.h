// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BASE
#define SSVSC_BASE

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SSVUtils/SSVUtils.h>
#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Body/CallbackInfo.h"
#include "SSVSCollision/Body/MassData.h"
#include "SSVSCollision/Body/GroupData.h"
#include "SSVSCollision/Spatial/SpatialInfoBase.h"
#include "SSVSCollision/Body/Enums.h"

namespace ssvsc
{
	class World;
	class Cell;
	struct ResolverBase;

	class Base
	{
		protected:
			World& world;
			SpatialInfoBase& spatialInfo;
			bool outOfBounds{false};

			Base(World& mWorld);

		public:
			virtual ~Base();

			virtual AABB& getShape() = 0;
			virtual AABB& getOldShape() = 0;
			virtual	void update(float mFrameTime) = 0;
			virtual void destroy() = 0;
			virtual const std::vector<int>& getGroupUidsToCheck() = 0;
			virtual Type getType() = 0;

			inline SpatialInfoBase& getSpatialInfo()		{ return spatialInfo; }
			inline void setOutOfBounds(bool mOutOfBounds)	{ outOfBounds = mOutOfBounds; }
	};
}

#endif
