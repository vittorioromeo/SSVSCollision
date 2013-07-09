// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <algorithm>
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
	void World::del(Base& mBase) { memoryManager.del(mBase); }

	World::World(ResolverBase& mResolver, SpatialBase& mSpatial) : resolver(&mResolver), spatial(&mSpatial) { }

	Body& World::create(Vec2i mPosition, Vec2i mSize, bool mIsStatic)
	{
		auto& result(memoryManager.create<Body>(*this, mIsStatic, mPosition, mSize));
		bodies.push_back(&result); return result;
	}
	Sensor& World::createSensor(Vec2i mPosition, Vec2i mSize)
	{
		auto& result(memoryManager.create<Sensor>(*this, mPosition, mSize));
		sensors.push_back(&result); return result;
	}

	void World::update(float mFrameTime)
	{
		bodies.erase(remove_if(begin(bodies), end(bodies), [](const Body* mBody){ return !mBody->isAlive(); }), end(bodies));
		sensors.erase(remove_if(begin(sensors), end(sensors), [](const Sensor* mSensor){ return !mSensor->isAlive(); }), end(sensors));

		memoryManager.refresh();
		for(const auto& b : memoryManager) b->update(mFrameTime);

		spatial->refresh();
	}
}
