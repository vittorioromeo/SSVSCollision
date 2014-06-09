// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BASE
#define SSVSC_BASE

namespace ssvsc
{
	class AABB;
	template<typename TW> class Body;

	template<typename TW> class Base : ssvu::NoCopy, public Groupable
	{
		public:
			using SpatialInfoType = typename TW::SpatialInfoType;
			using DetectionInfoType = typename TW::DetectionInfoType;
			using ResolverType = typename TW::ResolverType;
			using ResolverInfoType = typename TW::ResolverInfoType;
			using ResolutionInfoType = typename TW::ResolutionInfoType;

		protected:
			TW& world;
			SpatialInfoType spatialInfo;
			bool outOfBounds{false};

			inline Base(TW& mWorld) noexcept : world(mWorld), spatialInfo{world.spatial, *this} { }

		public:
			using Groupable::Groupable;

			ssvu::Delegate<void()> onPreUpdate;
			ssvu::Delegate<void(const DetectionInfoType&)> onDetection;

			inline void setOutOfBounds(bool mValue) noexcept					{ outOfBounds = mValue; }

			inline TW& getWorld() const noexcept								{ return world; }
			inline SpatialInfoType& getSpatialInfo() noexcept					{ return spatialInfo; }
			inline bool mustCheck(const Base& mBase) const noexcept				{ return mBase.hasAnyGroup(getGroupsToCheck()); }
			inline bool mustIgnoreResolution(const Base& mBase) const noexcept	{ return mBase.hasAnyGroup(getGroupsNoResolve()); }
	};
}

#endif
