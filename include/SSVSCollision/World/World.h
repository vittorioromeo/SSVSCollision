// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_WORLD
#define SSVSC_WORLD

#include <vector>
#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/Query/Query.h"


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
			World(ResolverBase& mResolver, SpatialBase& mSpatial) : resolver(&mResolver), spatial(&mSpatial) { }

			inline Body& create(Vec2i mPosition, Vec2i mSize, bool mIsStatic)
			{
				auto& result(bases.create<Body>(*this, mIsStatic, mPosition, mSize));
				bodies.push_back(&result); return result;
			}
			inline Sensor& createSensor(Vec2i mPosition, Vec2i mSize)
			{
				auto& result(bases.create<Sensor>(*this, mPosition, mSize));
				sensors.push_back(&result); return result;
			}

			void update(float mFrameTime);
			inline void clear() { bases.clear(); bodies.clear(); sensors.clear(); }

			inline decltype(bases)::Container& getBases()	{ return bases.getItems(); }
			inline ResolverBase& getResolver()				{ return *resolver; }
			inline SpatialBase& getSpatial()				{ return *spatial; }
			inline std::vector<Body*>& getBodies()			{ return bodies; } // TODO: remove?
			inline std::vector<Sensor*>& getSensors()		{ return sensors; } // TODO: remove?

			template<typename T> inline T& getResolver()	{ return static_cast<T&>(getResolver()); }
			template<typename T> inline T& getSpatial()		{ return static_cast<T&>(getSpatial()); }

			template<typename TSpatial, QueryType TType, QueryMode TMode = QueryMode::All, typename... TArgs>
			inline Query<TSpatial, typename QueryTypeDispatcher<TSpatial, TType>::Type, typename QueryModeDispatcher<TSpatial, TMode>::Type> getQuery(TArgs&&... mArgs)
			{
				return {getSpatial<TSpatial>(), std::forward<TArgs>(mArgs)...};
			}
	};

	// TODO: test collision pair generation with bodyA->priority bodyB->priority for retro resolver
}

#endif
