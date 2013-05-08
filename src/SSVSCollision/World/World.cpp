// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/World/World.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Utils/Utils.h"
#include "SSVSCollision/Resolver/ResolverBase.h"
#include "SSVSCollision/Spatial/SpatialBase.h"

using namespace std;
using namespace ssvsc::Utils;

namespace ssvsc
{
	World::World(ResolverBase& mResolver, SpatialBase& mSpatial) : resolver(mResolver), spatial(mSpatial) { }
	World::~World() { clear(); delete &resolver; delete &spatial; }

	Body& World::create(sf::Vector2i mPosition, sf::Vector2i mSize, bool mIsStatic)
	{
		return memoryManager.create(*this, mIsStatic, mPosition, mSize);
	}

	void World::del(Body* mBody) { memoryManager.del(mBody); memoryManager.cleanUp(); }
	void World::update(float mFrameTime)
	{
		memoryManager.cleanUp();
		for(auto& body : memoryManager.getItems()) body->update(mFrameTime);
	}
	void World::clear()
	{
		for(auto& body : memoryManager.getItems()) body->destroy();
		memoryManager.cleanUp();
	}

	vector<Body*>& World::getBodies()	{ return memoryManager.getItems(); }
	ResolverBase& World::getResolver()	{ return resolver; }
	SpatialBase& World::getSpatial()	{ return spatial; }
}
