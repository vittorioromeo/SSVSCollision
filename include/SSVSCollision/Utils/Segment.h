// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_UTILS_SEGMENT
#define SSVSC_UTILS_SEGMENT

#include <SFML/System.hpp>

template<typename T> struct Segment
{
	sf::Vector2<T> start, end;
	Segment(sf::Vector2<T> mStart, sf::Vector2<T> mEnd) : start{mStart}, end{mEnd} { }
};

#endif
