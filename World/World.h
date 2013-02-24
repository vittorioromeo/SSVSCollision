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
	class Body;
	class ResolverBase;
	class SpatialBase;

	class World
	{
		friend class Body;

		private:
			std::vector<Body*> bodies; // owned
			google::dense_hash_set<Body*> bodiesToDel;
			ResolverBase* resolver; // owned
			SpatialBase* spatial; // owned

			void add(Body* mBody);
			void del(Body* mBody);
		
		public:
			World(ResolverBase& mResolver, SpatialBase& mSpatial);
			~World();

			Body& create(sf::Vector2i mPosition, sf::Vector2i mSize, bool mIsStatic);
			void update(float mFrameTime);			

			ResolverBase& getResolver();
			SpatialBase& getSpatial();
	};
}

#endif // WORLD_H
