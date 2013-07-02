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
#include "SSVSCollision/World/GroupManager.h"

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
	class Base;
	class Body;
	class Sensor;
	struct ResolverBase;
	struct SpatialBase;

	class World
	{
		friend class Base;
		friend class Body;
		friend class Sensor;

		private:
			GroupManager groupManager;
			ssvu::MemoryManager<Base, std::vector<Base*>, google::dense_hash_set<Base*>> memoryManager;
			ResolverBase& resolver; // owned
			SpatialBase& spatial; // owned

			std::vector<Body*> bodies;

			void delBase(Base* mBase);
			void delBody(Body* mBody);

		public:
			World(ResolverBase& mResolver, SpatialBase& mSpatial);
			~World();

			Body& create(sf::Vector2i mPosition, sf::Vector2i mSize, bool mIsStatic);
			Sensor& createSensor(sf::Vector2i mPosition, sf::Vector2i mSize);

			void update(float mFrameTime);
			void clear();

			inline GroupManager& getGroupManager()	{ return groupManager; }
			inline std::vector<Base*>& getBases()	{ return memoryManager.getItems(); }
			inline std::vector<Body*>& getBodies()	{ return bodies; }
			inline ResolverBase& getResolver()		{ return resolver; }
			inline SpatialBase& getSpatial()		{ return spatial; }

			inline int getGroupUid(const std::string& mGroup) { return groupManager.getUid(mGroup); }

			template<typename T> T& getResolver()	{ return static_cast<T&>(getResolver()); }
			template<typename T> T& getSpatial()	{ return static_cast<T&>(getSpatial()); }
	};
}

#endif
