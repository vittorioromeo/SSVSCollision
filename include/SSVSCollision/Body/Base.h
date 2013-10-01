// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BASE
#define SSVSC_BASE

#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/World/World.h"
#include "SSVSCollision/Body/Groupable.h"

namespace ssvsc
{
	class AABB;
	template<typename TW> class Body;

	template<typename TW> class Base : public ssvu::MemoryManageable, public Groupable
	{
		public:
			using BaseType = Base<TW>;
			using BodyType = Body<TW>;
			using SpatialInfoType = typename TW::SpatialInfoType;
			using DetectionInfoType = typename TW::DetectionInfoType;

		protected:
			TW& world;
			SpatialInfoType spatialInfo;
			bool outOfBounds{false};

			inline Base(TW& mWorld) noexcept : world(mWorld), spatialInfo(world.spatial, *this) { }

		public:
			using Groupable::Groupable;

			ssvu::Delegate<void()> onPreUpdate;
			ssvu::Delegate<void(const DetectionInfoType&)> onDetection;

			inline virtual ~Base() noexcept { }

			virtual	void update(float mFT) = 0;
			virtual void handleCollision(float mFT, BodyType* mBody) = 0;
			inline virtual void destroy() { world.del(*this); }

			inline void setOutOfBounds(bool mOutOfBounds) noexcept { outOfBounds = mOutOfBounds; }

			virtual AABB& getShape() noexcept = 0;
			virtual AABB& getOldShape() noexcept = 0;
			virtual BBType getType() const noexcept = 0;
			inline SpatialInfoType& getSpatialInfo() noexcept	{ return spatialInfo; }
			inline TW& getWorld() const noexcept				{ return world; }

			inline bool mustCheck(const BaseType& mBase) const noexcept				{ return mBase.hasAnyGroup(getGroupsToCheck()); }
			inline bool mustIgnoreResolution(const BaseType& mBase) const noexcept	{ return mBase.hasAnyGroup(getGroupsNoResolve()); }
	};
}

#endif
