#include "Body.h"

using namespace std;
using namespace sf;

namespace ssvsc
{
	AABB& Body::getShape()								{ return shape; }
	AABB& Body::getOldShape() 							{ return oldShape; }
	void Body::setPosition(Vector2i mPosition) 			{ shape.setPosition(mPosition); }
	void Body::setVelocity(Vector2i mVelocity) 			{ velocity = mVelocity; }
	void Body::setStatic(bool mStatic) 					{ isStatic = mStatic; }
	void Body::setUserData(void* mUserData) 			{ userData = mUserData; }
	const vector<string>& Body::getGroups()				{ return groups; }
	const vector<string>& Body::getGroupsToCheck()		{ return groupsToCheck; }
	Vector2i Body::getPosition() 						{ return shape.getPosition(); }
	Vector2i Body::getVelocity() 						{ return velocity; }
	bool Body::getStatic() 								{ return isStatic; }
	void* Body::getUserData()							{ return userData; }
}
