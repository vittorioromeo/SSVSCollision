// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_WORLD
#define SSVSC_WORLD

#include "SSVSCollision/Global/Typedefs.hpp"
#include "SSVSCollision/Query/Query.hpp"

namespace ssvsc
{
	template<typename TW> class Base;
	template<typename TW> class Body;
	template<typename TW> class Sensor;
	template<typename TW> struct DetectionInfo;
	template<typename TW> struct ResolutionInfo;

	template<template<typename> class TS, template<typename> class TR> class World : ssvu::NoCopy
	{
		public:
			using SpatialType = TS<World>;
			using SpatialInfoType = typename SpatialType::SpatialInfoType;
			using ResolverType = TR<World>;
			using ResolverInfoType = typename ResolverType::ResolverInfoType;
			using BaseType = Base<World>;
			using BodyType = Body<World>;
			using SensorType = Sensor<World>;
			using DetectionInfoType = DetectionInfo<World>;
			using ResolutionInfoType = ResolutionInfo<World>;
			friend BaseType;
			friend BodyType;
			friend SensorType;

		private:
			ssvu::MemoryManager<BodyType> bodies;
			ssvu::MemoryManager<SensorType> sensors;

			SpatialType spatial;
			ResolverType resolver;

			inline void delBody(BodyType& mBase) noexcept		{ bodies.del(mBase); }
			inline void delSensor(SensorType& mBase) noexcept	{ sensors.del(mBase); }

		public:
			template<typename... TArgs> inline World(TArgs&&... mArgs) : spatial{std::forward<TArgs>(mArgs)...} { }

			inline BodyType& create(const Vec2i& mPos, const Vec2i& mSize, bool mStatic)	{ return bodies.create(*this, mStatic, mPos, mSize); }
			inline SensorType& createSensor(const Vec2i& mPos, const Vec2i& mSize)			{ return sensors.create(*this, mPos, mSize); }

			inline void update(FT mFT)
			{
				bodies.refresh();
				sensors.refresh();
				for(const auto& b : bodies) b->update(mFT);
				for(const auto& s : sensors) s->update(mFT);
				resolver.postUpdate(*this);
			}
			inline void clear() noexcept { bodies.clear(); sensors.clear(); }

			inline const decltype(bodies)& getBodies() const noexcept	{ return bodies; }
			inline const decltype(sensors)& getSensors() const noexcept	{ return sensors; }
			inline const SpatialType& getSpatial() const noexcept		{ return spatial; }
			inline const ResolverType& getResolver() const noexcept		{ return resolver; }

			template<QueryType TType, QueryMode TMode = QueryMode::All, typename... TArgs>
			inline Query<World, typename QueryTypeDispatcher<World, SpatialType, TType>::Type, typename QueryModeDispatcher<World, SpatialType, TMode>::Type> getQuery(TArgs&&... mArgs) noexcept
			{
				return {spatial, std::forward<TArgs>(mArgs)...};
			}
	};
}

#endif
