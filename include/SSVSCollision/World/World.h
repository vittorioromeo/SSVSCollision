// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_WORLD
#define SSVSC_WORLD

#include <vector>
#include "SSVSCollision/Global/Typedefs.h"

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
			ssvu::MemoryManager<Base> bases;

			Uptr<ResolverBase> resolver;
			Uptr<SpatialBase> spatial;

			std::vector<Body*> bodies; // TODO: remove?
			std::vector<Sensor*> sensors; // TODO: remove?

			inline void del(Base& mBase) { bases.del(mBase); }

		public:
			World(ResolverBase& mResolver, SpatialBase& mSpatial);

			Body& create(Vec2i mPosition, Vec2i mSize, bool mIsStatic);
			Sensor& createSensor(Vec2i mPosition, Vec2i mSize);

			void update(float mFrameTime);
			inline void clear() { bases.clear(); bodies.clear(); sensors.clear(); }

			inline decltype(bases)::Container& getBases()	{ return bases.getItems(); }
			inline ResolverBase& getResolver()				{ return *resolver; }
			inline SpatialBase& getSpatial()				{ return *spatial; }
			inline std::vector<Body*>& getBodies()			{ return bodies; } // TODO: remove?
			inline std::vector<Sensor*>& getSensors()		{ return sensors; } // TODO: remove?

			template<typename T> inline T& getResolver()	{ return static_cast<T&>(getResolver()); }
			template<typename T> inline T& getSpatial()		{ return static_cast<T&>(getSpatial()); }
	};
}

#endif
