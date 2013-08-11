// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <SSVUtils/SSVUtils.h>
#include "SSVSCollision/World/World.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Body/Sensor.h"
#include "SSVSCollision/Resolver/ResolverBase.h"
#include "SSVSCollision/Spatial/SpatialBase.h"

using namespace std;
using namespace ssvu;

namespace ssvsc
{
	World::World(ResolverBase& mResolver, SpatialBase& mSpatial) : resolver(&mResolver), spatial(&mSpatial) { }

	Body& World::create(Vec2i mPosition, Vec2i mSize, bool mIsStatic)
	{
		auto& result(bases.create<Body>(*this, mIsStatic, mPosition, mSize));
		bodies.push_back(&result); return result;
	}
	Sensor& World::createSensor(Vec2i mPosition, Vec2i mSize)
	{
		auto& result(bases.create<Sensor>(*this, mPosition, mSize));
		sensors.push_back(&result); return result;
	}

	void World::update(float mFrameTime)
	{
		eraseRemoveIf(bodies, &bases.isDead<Body*>);
		eraseRemoveIf(sensors, &bases.isDead<Sensor*>);

		bases.refresh();
		for(const auto& b : bases) b->update(mFrameTime);

		spatial->refresh();
	}
}
