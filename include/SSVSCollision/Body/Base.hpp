// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BASE
#define SSVSC_BASE

namespace ssvsc
{
class AABB;
template <typename TW>
class Body;

template <typename TW>
class Base : public Groupable
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

    inline Base(TW& mWorld) noexcept : world(mWorld),
                                       spatialInfo{world.spatial, *this}
    {
    }

public:
    using Groupable::Groupable;

    inline Base() = default;

    inline Base(const Base&) = delete;
    inline Base& operator=(const Base&) = delete;

    ssvu::Delegate<void()> onPreUpdate;
    ssvu::Delegate<void(const DetectionInfoType&)> onDetection;

    inline void setOutOfBounds(bool mValue) noexcept { outOfBounds = mValue; }

    inline auto& getWorld() const noexcept { return world; }
    inline auto& getSpatialInfo() noexcept { return spatialInfo; }
    inline bool mustCheck(const Base& mX) const noexcept
    {
        return mX.hasAnyGroup(getGroupsToCheck());
    }
    inline bool mustIgnoreResolution(const Base& mX) const noexcept
    {
        return mX.hasAnyGroup(getGroupsNoResolve());
    }
};
}

#endif
