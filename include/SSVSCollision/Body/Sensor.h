// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SENSOR
#define SSVSC_SENSOR

#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Body/CallbackInfo.h"
#include "SSVSCollision/Body/GroupData.h"
#include "SSVSCollision/Spatial/SpatialInfoBase.h"
#include "SSVSCollision/Body/Base.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/World/World.h"

namespace ssvsc
{
	class World;

	class Sensor : public Base
	{
		private:
			AABB shape;

		public:
			ssvu::Delegate<void> onPreUpdate;
			ssvu::Delegate<void, DetectionInfo> onDetection;

			Sensor(World& mWorld, Vec2i mPosition, Vec2i mSize) : Base(mWorld), shape{mPosition, mSize / 2} { }
			~Sensor() { spatialInfo.destroy(); }

			void update(float mFrameTime) override;
			void handleCollision(float mFrameTime, Body* mBody) override;
			inline AABB& getShape() override	{ return shape; }
			inline AABB& getOldShape() override	{ return shape; }
			inline Type getType() override		{ return Type::Sensor; }
			void destroy() override				{ world.delSensor(this); Base::destroy(); }

			void addGroupsToCheck(const std::vector<std::string>& mGroups);


			inline void setPosition(Vec2i mPosition)
			{
				if(mPosition != shape.getPosition()) spatialInfo.invalidate();
				shape.setPosition(mPosition);
			}

			inline GroupData& getGroupData()	{ return groupData; }

			inline bool mustCheck(Body& mBody)	{ return (groupData.getGroupsToCheck() & mBody.getGroups()).any(); }
	};
}

#endif

