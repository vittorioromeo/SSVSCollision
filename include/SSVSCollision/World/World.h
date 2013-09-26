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
	template<typename> class BaseType;
	template<typename> class BodyType;
	template<typename> class SensorType;
	template<typename> struct ResolverBase;

	template<typename TS> class World
	{
		template<typename> friend class BaseType;
		template<typename> friend class BodyType;
		template<typename> friend class SensorType;

		public:
			using Base = BaseType<TS>;
			using Body = BodyType<TS>;
			using Sensor = SensorType<TS>;
			using Resolver = ResolverBase<TS>;
			using DI = DetectionInfo<TS>;
			using RI = ResolutionInfo<TS>;
			using SI = typename TS::TSInfo;

		private:
			ssvu::MemoryManager<Base> bases;

			Uptr<Resolver> resolver;
			TS spatial;

			std::vector<Body*> bodies;
			std::vector<Sensor*> sensors;

			inline void del(Base& mBase) { bases.del(mBase); }

		public:
			template<typename... TArgs> inline World(Resolver& mResolver, TArgs&&... mArgs) : resolver(&mResolver), spatial(std::forward<TArgs>(mArgs)...) { }

			inline Body& create(const Vec2i& mPosition, const Vec2i& mSize, bool mIsStatic)
			{
				auto& result(bases.template create<Body>(*this, mIsStatic, mPosition, mSize));
				bodies.push_back(&result); return result;
			}
			inline Sensor& createSensor(const Vec2i& mPosition, const Vec2i& mSize)
			{
				auto& result(bases.template create<Sensor>(*this, mPosition, mSize));
				sensors.push_back(&result); return result;
			}

			inline void update(float mFT)
			{
				ssvu::eraseRemoveIf(bodies, &bases.template isDead<Body*>);
				ssvu::eraseRemoveIf(sensors, &bases.template isDead<Sensor*>);

				bases.refresh();
				for(const auto& b : bases) b->update(mFT);
				resolver->postUpdate(*this);
			}
			inline void clear() { bases.clear(); bodies.clear(); sensors.clear(); }

			inline typename ssvu::MemoryManager<Base>::Container& getBases() noexcept	{ return bases.getItems(); }
			inline Resolver& getResolver() noexcept				{ return *resolver; }
			inline TS& getSpatial() noexcept					{ return spatial; }
			inline std::vector<Body*>& getBodies() noexcept			{ return bodies; }
			inline std::vector<Sensor*>& getSensors() noexcept		{ return sensors; }

			template<typename T> inline T& getResolver()	{ return static_cast<T&>(getResolver()); }

			template<QueryType TType, QueryMode TMode = QueryMode::All, typename... TArgs>
			inline Query<TS, typename QueryTypeDispatcher<TS, TType>::Type, typename QueryModeDispatcher<TS, TMode>::Type> getQuery(TArgs&&... mArgs)
			{
				return {spatial, std::forward<TArgs>(mArgs)...};
			}
	};

	// TODO: templatize?
}

#endif
