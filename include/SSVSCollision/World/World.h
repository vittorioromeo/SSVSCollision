// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_WORLD
#define SSVSC_WORLD

#include <vector>
#include <SFML/Graphics.hpp>
#include <SSVStart/SSVStart.h>
#pragma GCC system_header
#include <google/dense_hash_set>

namespace ssvu
{
	namespace Traits
	{
		#ifndef SSVU_TRAITS_DENSEHASHSET
		#define SSVU_TRAITS_DENSEHASHSET
		template<typename TItem> struct Container<google::dense_hash_set<TItem>, TItem>
		{
			typedef google::dense_hash_set<TItem> TContainer;

			static void init(TContainer& mContainer) { mContainer.set_empty_key(nullptr); }
			static void clear(TContainer& mContainer) { mContainer.clear(); }
			static void add(TContainer& mContainer, const TItem& mItem) { mContainer.insert(mItem); }
			static void del(TContainer&, const TItem&) { }
		};
		#endif
	}
}

namespace ssvsc
{
	class Body;
	class ResolverBase;
	class SpatialBase;

	class World
	{
		friend class Body;

		private:
			ssvu::MemoryManager<Body, std::vector<Body*>, google::dense_hash_set<Body*>> memoryManager;
			ResolverBase& resolver; // owned
			SpatialBase& spatial; // owned

			void del(Body* mBody);

		public:
			World(ResolverBase& mResolver, SpatialBase& mSpatial);
			~World();

			Body& create(sf::Vector2i mPosition, sf::Vector2i mSize, bool mIsStatic);
			void update(float mFrameTime);
			void clear();

			std::vector<Body*>& getBodies();
			ResolverBase& getResolver();
			SpatialBase& getSpatial();

			template<typename T> T& getResolver() { return static_cast<T&>(getResolver()); }
			template<typename T> T& getSpatial() { return static_cast<T&>(getSpatial()); }
	};
}

#endif
