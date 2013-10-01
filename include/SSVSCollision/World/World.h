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
	template<typename TW> class Base;
	template<typename TW> class Body;
	template<typename TW> class Sensor;
	template<typename TW> class DetectionInfo;
	template<typename TW> class ResolutionInfo;

	template<template<typename> class TS, template<typename> class TR> class World
	{
		public:
			using SpatialType = TS<World>;
			using ResolverType = TR<World>;
			using SpatialInfoType = typename SpatialType::SpatialInfoType;
			using BaseType = Base<World>;
			using BodyType = Body<World>;
			using SensorType = Sensor<World>;
			using DetectionInfoType = DetectionInfo<World>;
			using ResolutionInfoType = ResolutionInfo<World>;
			friend BaseType;
			friend BodyType;
			friend SensorType;

		private:
			ssvu::MemoryManager<BaseType> bases;
			std::vector<BodyType*> bodies;
			std::vector<SensorType*> sensors;
			SpatialType spatial;
			ResolverType resolver;

			inline void del(BaseType& mBase) { bases.del(mBase); }

		public:
			template<typename... TArgs> inline World(TArgs&&... mArgs) : spatial(std::forward<TArgs>(mArgs)...) { }

			inline BodyType& create(const Vec2i& mPos, const Vec2i& mSize, bool mStatic)	{ auto& result(bases.template create<BodyType>(*this, mStatic, mPos, mSize)); bodies.push_back(&result); return result; }
			inline SensorType& createSensor(const Vec2i& mPos, const Vec2i& mSize)			{ auto& result(bases.template create<SensorType>(*this, mPos, mSize)); sensors.push_back(&result); return result; }

			inline void update(float mFT)
			{
				ssvu::eraseRemoveIf(bodies, &bases.template isDead<BodyType*>);
				ssvu::eraseRemoveIf(sensors, &bases.template isDead<SensorType*>);

				bases.refresh();
				for(const auto& b : bases) b->update(mFT);
				resolver.postUpdate(*this);
			}
			inline void clear() { bases.clear(); bodies.clear(); sensors.clear(); }

			inline const SpatialType& getSpatial() const noexcept						{ return spatial; }
			inline const ResolverType& getResolver() const noexcept						{ return resolver; }
			inline const decltype(bodies)& getBodies() const noexcept					{ return bodies; }
			inline const decltype(sensors)& getSensors() const noexcept					{ return sensors; }
			inline const typename decltype(bases)::Container& getBases() const noexcept	{ return bases.getItems(); }


			template<QueryType TType, QueryMode TMode = QueryMode::All, typename... TArgs>
			inline Query<World, typename QueryTypeDispatcher<World, SpatialType, TType>::Type, typename QueryModeDispatcher<World, SpatialType, TMode>::Type> getQuery(TArgs&&... mArgs) noexcept
			{
				return {spatial, std::forward<TArgs>(mArgs)...};
			}
	};
}

#endif
