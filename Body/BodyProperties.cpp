// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Body.h"

using namespace std;
using namespace sf;

namespace ssvsc
{
	void Body::setPosition(Vector2i mPosition) 			{ shape.setPosition(mPosition); gridInfo.invalidate(); }
	void Body::setVelocity(Vector2f mVelocity) 			{ velocity = mVelocity; }
	void Body::setAcceleration(Vector2f mAcceleration)	{ acceleration = mAcceleration; }
	void Body::setStatic(bool mStatic) 					{ _static = mStatic; }
	void Body::setUserData(void* mUserData) 			{ userData = mUserData; }
	void Body::setX(int mX)				 				{ shape.setX(mX); gridInfo.invalidate(); }
	void Body::setY(int mY)				 				{ shape.setY(mY); gridInfo.invalidate(); }
	void Body::setVelocityX(float mX)				 	{ velocity.x = mX; }
	void Body::setVelocityY(float mY)				 	{ velocity.y = mY; }
	void Body::setSize(Vector2i mSize)					{ shape.setSize(mSize); gridInfo.invalidate(); }
	void Body::setWidth(int mWidth)						{ shape.setWidth(mWidth); gridInfo.invalidate(); }
	void Body::setHeight(int mHeight)					{ shape.setHeight(mHeight); gridInfo.invalidate(); }
	void Body::setOutOfBounds(bool mOutOfBounds)		{ outOfBounds = mOutOfBounds; }

	AABB& Body::getShape()								{ return shape; }
	AABB& Body::getOldShape() 							{ return oldShape; }
	const vector<string>& Body::getGroups()				{ return groups; }
	const vector<string>& Body::getGroupsToCheck()		{ return groupsToCheck; }
	const vector<string>& Body::getGroupsNoResolve()	{ return groupsNoResolve; }
	Vector2i Body::getPosition() const					{ return shape.getPosition(); }
	Vector2f Body::getVelocity() const					{ return velocity; }
	Vector2f Body::getAcceleration() const				{ return acceleration; }
	Vector2i Body::getSize() const						{ return shape.getSize(); }
	int Body::getWidth() const							{ return shape.getWidth(); }
	int Body::getHeight() const							{ return shape.getHeight(); }
	bool Body::isStatic() const							{ return _static; }
	void* Body::getUserData() const						{ return userData; }
	bool Body::hasMovedLeft() const						{ return shape.getX() < oldShape.getX(); }
	bool Body::hasMovedRight() const					{ return shape.getX() > oldShape.getX(); }
	bool Body::hasMovedUp() const						{ return shape.getY() < oldShape.getY(); }
	bool Body::hasMovedDown() const						{ return shape.getY() < oldShape.getY(); }
}
