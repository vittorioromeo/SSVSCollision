// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSCOLLISION_GLOBAL_TYPEDEFS
#define SSVSCOLLISION_GLOBAL_TYPEDEFS

#include <SSVStart/SSVStart.h>
#include <bitset>

namespace ssvsc
{
	using Group = unsigned int;
	using Bitset = std::bitset<64>;
	using Vec2i = ssvs::Vec2i;
	using Vec2f = ssvs::Vec2f;
	template<typename T> using Vec2 = ssvs::Vec2<T>;

	enum class Type{Body, Sensor};
}

#endif
