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
using namespace sf;
using namespace ssvu;

namespace ssvsc
{
	void World::delBase(Base* mBase)		{ memoryManager.del(mBase); memoryManager.cleanUp(); }
	void World::delBody(Body* mBody)		{ eraseRemove(bodies, mBody); delBase(mBody); }
	void World::delSensor(Sensor* mSensor)	{ eraseRemove(sensors, mSensor); delBase(mSensor); }

	World::World(ResolverBase& mResolver, SpatialBase& mSpatial) : resolver(mResolver), spatial(mSpatial) { }
	World::~World() { delete &resolver; delete &spatial; }

	Body& World::create(Vector2i mPosition, Vector2i mSize, bool mIsStatic)
	{
		auto& result(memoryManager.create<Body>(*this, mIsStatic, mPosition, mSize));
		bodies.push_back(&result); return result;
	}
	Sensor& World::createSensor(Vector2i mPosition, Vector2i mSize)
	{
		auto& result(memoryManager.create<Sensor>(*this, mPosition, mSize));
		sensors.push_back(&result); return result;
	}

	void World::update(float mFrameTime)
	{
		memoryManager.cleanUp();
		for(const auto& e : memoryManager) e->update(mFrameTime);
	}
	void World::clear()
	{
		for(const auto& e : memoryManager) e->destroy();
		memoryManager.cleanUp();
	}
}
