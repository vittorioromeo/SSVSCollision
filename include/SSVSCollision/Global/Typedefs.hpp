// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSCOLLISION_GLOBAL_TYPEDEFS
#define SSVSCOLLISION_GLOBAL_TYPEDEFS

#include <bitset>
#include <memory>
#include <SSVStart/SSVStart.hpp>

namespace ssvsc
{
	constexpr std::size_t maxGroups = 32;
	using Group = unsigned int;
	using GroupBitset = std::bitset<maxGroups>;

	using Vec2i = ssvs::Vec2i;
	using Vec2f = ssvs::Vec2f;
	using FT = ssvu::FT;
	template<typename T, typename TDeleter = std::default_delete<T>> using Uptr = ssvs::Uptr<T, TDeleter>;
	template<typename T> using Vec2 = ssvs::Vec2<T>;

	enum class QueryType{Point, Distance, RayCast, OrthoLeft, OrthoRight, OrthoUp, OrthoDown};
	enum class QueryMode{All, ByGroup};

	class BodyTag { };
	class SensorTag { };
}

#endif
