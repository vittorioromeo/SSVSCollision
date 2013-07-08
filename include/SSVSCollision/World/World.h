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

			std::vector<Uptr<Base>> bases;

			std::vector<Base*> toAddBases;
			std::vector<Body*> toAddBodies;
			std::vector<Sensor*> toAddSensors;

			ResolverBase& resolver; // owned
			SpatialBase& spatial; // owned

			std::vector<Body*> bodies; // TODO: remove?
			std::vector<Sensor*> sensors; // TODO: remove?

		public:
			World(ResolverBase& mResolver, SpatialBase& mSpatial);
			~World();

			Body& create(Vec2i mPosition, Vec2i mSize, bool mIsStatic);
			Sensor& createSensor(Vec2i mPosition, Vec2i mSize);

			void update(float mFrameTime);
			void clear();

			inline GroupManager& getGroupManager()		{ return groupManager; }
			inline std::vector<Uptr<Base>>& getBases()	{ return bases; }
			inline ResolverBase& getResolver()			{ return resolver; }
			inline SpatialBase& getSpatial()			{ return spatial; }
			inline std::vector<Body*>& getBodies()		{ return bodies; } // TODO: remove?
			inline std::vector<Sensor*>& getSensors()	{ return sensors; } // TODO: remove?

			template<typename T> T& getResolver()		{ return static_cast<T&>(getResolver()); }
			template<typename T> T& getSpatial()		{ return static_cast<T&>(getSpatial()); }

			inline Group getGroup(const std::string& mLabel) { return groupManager.get(mLabel); }
	};
}

#endif
