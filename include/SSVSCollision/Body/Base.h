// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BASE
#define SSVSC_BASE

#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/World/World.h"
#include "SSVSCollision/Spatial/SpatialBase.h"
#include "SSVSCollision/Body/Groupable.h"

namespace ssvsc
{
	class AABB;
	class Body;
	class SpatialInfoBase;

	class Base : public ssvu::MemoryManageable, public Groupable
	{
		protected:
			World& world;
			SpatialInfoBase& spatialInfo;
			bool outOfBounds{false};

			inline Base(World& mWorld) noexcept : world(mWorld), spatialInfo(world.getSpatial().createSpatialInfo(*this)) { }

		public:
			using Groupable::Groupable;

			ssvu::Delegate<void()> onPreUpdate;
			ssvu::Delegate<void(const DetectionInfo&)> onDetection;

			inline virtual ~Base() noexcept { }

			virtual	void update(float mFT) = 0;
			virtual void handleCollision(float mFT, Body* mBody) = 0;
			inline virtual void destroy() { world.del(*this); }

			inline void setOutOfBounds(bool mOutOfBounds) noexcept { outOfBounds = mOutOfBounds; }

			virtual AABB& getShape() noexcept = 0;
			virtual AABB& getOldShape() noexcept = 0;
			virtual BaseType getType() const noexcept = 0;
			inline SpatialInfoBase& getSpatialInfo() noexcept	{ return spatialInfo; }
			inline World& getWorld() const noexcept				{ return world; }

			inline bool mustCheck(const Base& mBase) const noexcept				{ return mBase.hasAnyGroup(getGroupsToCheck()); }
			inline bool mustIgnoreResolution(const Base& mBase) const noexcept	{ return mBase.hasAnyGroup(getGroupsNoResolve()); }
	};
}

#endif
