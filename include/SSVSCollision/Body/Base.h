// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BASE
#define SSVSC_BASE

#include <SFML/System.hpp>
#include "SSVSCollision/Body/Enums.h"

namespace ssvsc
{
	class World;
	class Cell;
	class SpatialInfoBase;
	class AABB;
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
			virtual void destroy();
			virtual const std::vector<int>& getGroupUidsToCheck() = 0;
			virtual Type getType() = 0;

			inline SpatialInfoBase& getSpatialInfo()		{ return spatialInfo; }
			inline void setOutOfBounds(bool mOutOfBounds)	{ outOfBounds = mOutOfBounds; }
	};
}

#endif
