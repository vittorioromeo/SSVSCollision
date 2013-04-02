// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SEGMENT
#define SSVSC_SEGMENT

#include <SFML/System.hpp>

template<typename T> class Segment
{
	private:
		sf::Vector2<T> start, end;
	
	public:
		Segment(sf::Vector2<T> mStart, sf::Vector2<T> mEnd) : start{mStart}, end{mEnd} { }
		
		sf::Vector2<T> getStart() const { return start; }
		sf::Vector2<T> getEnd() const { return end; }
};

#endif
