// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <sparsehash/dense_hash_set>
#include <SFML/Graphics.hpp>

namespace ssvsc
{
	class SpatialBase;
	class Body;

	class World
	{
		friend class Body;

		private:
			std::vector<Body*> bodies; // owned
			google::dense_hash_set<Body*> bodiesToDel;
			SpatialBase* spatial;

			World(SpatialBase* mSpatial);
			void add(Body* mBody);
			void del(Body* mBody);
		
		public:
			template<typename TSpatial, typename... TArgs> static World& create(TArgs&&... mArgs)
			{
				World* result{new World(new TSpatial(std::forward<TArgs>(mArgs)...))};
				return *result;
			}
			~World();

			Body& create(sf::Vector2i mPosition, sf::Vector2i mSize, bool mIsStatic);
			void update(float mFrameTime);			

			SpatialBase& getSpatial();
	};
}

#endif // WORLD_H
