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
	World::World(ResolverBase& mResolver, SpatialBase& mSpatial) : resolver(mResolver), spatial(mSpatial) { }
	World::~World() { delete &resolver; delete &spatial; }

	Body& World::create(Vec2i mPosition, Vec2i mSize, bool mIsStatic)
	{
		auto result(new Body{*this, mIsStatic, mPosition, mSize});
		toAddBases.push_back(result);
		toAddBodies.push_back(result);
		return *result;
	}
	Sensor& World::createSensor(Vec2i mPosition, Vec2i mSize)
	{
		auto result(new Sensor{*this, mPosition, mSize});
		toAddBases.push_back(result);
		toAddSensors.push_back(result);
		return *result;
	}

	void World::update(float mFrameTime)
	{
		bodies.erase(remove_if(begin(bodies), end(bodies), [](const Body* mBody){ return !mBody->isAlive(); }), end(bodies));
		sensors.erase(remove_if(begin(sensors), end(sensors), [](const Sensor* mSensor){ return !mSensor->isAlive(); }), end(sensors));
		bases.erase(remove_if(begin(bases), end(bases), [](const Uptr<Base>& mBase){ return !mBase->isAlive(); }), end(bases));

		for(const auto& b : bases) b->update(mFrameTime);

		for(const auto& b : toAddBodies) bodies.push_back(b);
		for(const auto& b : toAddSensors) sensors.push_back(b);
		for(const auto& b : toAddBases) bases.push_back(Uptr<Base>(b));

		toAddBodies.clear();
		toAddSensors.clear();
		toAddBases.clear();
	}
	void World::clear()
	{
		bodies.clear();
		sensors.clear();
		bases.clear();
	}
}
