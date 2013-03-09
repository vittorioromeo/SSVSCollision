// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <google/dense_hash_set>
#include <SFML/Graphics.hpp>
#include <SSVStart.h>

namespace ssvsc
{
	class Body;
	class ResolverBase;
	class SpatialBase;

	class World
	{
		friend class Body;

		private:
			ssvs::Utils::MemoryManager<Body, std::vector<Body*>, google::dense_hash_set<Body*>> memoryManager;
			ResolverBase* resolver; // owned
			SpatialBase* spatial; // owned

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
