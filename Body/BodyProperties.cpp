// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Body.h"

using namespace std;
using namespace sf;

namespace ssvsc
{
	void Body::setX(int mX)				 				{ shape.setX(mX); gridInfo.invalidate(); }
	void Body::setY(int mY)				 				{ shape.setY(mY); gridInfo.invalidate(); }
	void Body::setVelocityX(float mX)				 	{ velocity.x = mX; }
	void Body::setVelocityY(float mY)				 	{ velocity.y = mY; }
	void Body::setPosition(Vector2i mPosition) 			{ shape.setPosition(mPosition); gridInfo.invalidate(); }
	void Body::setVelocity(Vector2f mVelocity) 			{ velocity = mVelocity; }
	void Body::setAcceleration(Vector2f mAcceleration)	{ acceleration = mAcceleration; }
	void Body::setStatic(bool mStatic) 					{ isStatic = mStatic; }
	void Body::setUserData(void* mUserData) 			{ userData = mUserData; }
	const vector<string>& Body::getGroups()				{ return groups; }
	const vector<string>& Body::getGroupsToCheck()		{ return groupsToCheck; }
	AABB& Body::getShape()								{ return shape; }
	AABB& Body::getOldShape() 							{ return oldShape; }
	Vector2i Body::getPosition() const					{ return shape.getPosition(); }
	Vector2f Body::getVelocity() const					{ return velocity; }
	Vector2f Body::getAcceleration() const				{ return acceleration; }
	bool Body::getStatic() const						{ return isStatic; }
	void* Body::getUserData() const						{ return userData; }
	bool Body::hasMovedLeft() const						{ return shape.getX() < oldShape.getX(); }
	bool Body::hasMovedRight() const					{ return shape.getX() > oldShape.getX(); }
	bool Body::hasMovedUp() const						{ return shape.getY() < oldShape.getY(); }
	bool Body::hasMovedDown() const						{ return shape.getY() < oldShape.getY(); }
}
