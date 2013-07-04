// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_WORLD
#define SSVSC_WORLD

#include <vector>
#include <google/dense_hash_set>
#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/Utils/Traits.h"
#include "SSVSCollision/Utils/GroupManager.h"
#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"

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

			std::vector<Body*> bodies; // TODO: remove?
			std::vector<Sensor*> sensors; // TODO: remove?

			void delBase(Base* mBase);
			void delBody(Body* mBody);
			void delSensor(Sensor* mSensor);

		public:
			World(ResolverBase& mResolver, SpatialBase& mSpatial);
			~World();

			Body& create(Vec2i mPosition, Vec2i mSize, bool mIsStatic);
			Sensor& createSensor(Vec2i mPosition, Vec2i mSize);

			void update(float mFrameTime);
			void clear();

			inline GroupManager& getGroupManager()		{ return groupManager; }
			inline std::vector<Base*>& getBases()		{ return memoryManager.getItems(); }
			inline ResolverBase& getResolver()			{ return resolver; }
			inline SpatialBase& getSpatial()			{ return spatial; }
			inline std::vector<Body*>& getBodies()		{ return bodies; } // TODO: remove?
			inline std::vector<Sensor*>& getSensors()	{ return sensors; } // TODO: remove?

			template<typename T> T& getResolver()		{ return static_cast<T&>(getResolver()); }
			template<typename T> T& getSpatial()		{ return static_cast<T&>(getSpatial()); }

			inline int getGroup(const std::string& mLabel) { return groupManager.get(mLabel); }
	};
}

#endif
