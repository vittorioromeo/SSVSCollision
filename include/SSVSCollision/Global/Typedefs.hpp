// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSCOLLISION_GLOBAL_TYPEDEFS
#define SSVSCOLLISION_GLOBAL_TYPEDEFS

namespace ssvsc
{
using ssvu::SizeT;
using ssvu::FT;
using ssvs::Vec2;
using ssvs::Vec2i;
using ssvs::Vec2f;
using ssvs::UPtr;

constexpr SizeT maxGroups{32};
using Group = unsigned int;
using GroupBitset = std::bitset<maxGroups>;

enum class QueryType
{
    Point,
    Distance,
    RayCast,
    OrthoLeft,
    OrthoRight,
    OrthoUp,
    OrthoDown
};
enum class QueryMode
{
    All,
    ByGroup
};

struct BodyTag
{
};
struct SensorTag
{
};
}

#endif
