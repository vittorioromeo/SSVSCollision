#include "Body.h"

using namespace std;
using namespace sf;

namespace ssvsc
{
	void Body::setPosition(Vector2i mPosition) 			{ position = mPosition; }
	void Body::setVelocity(Vector2i mVelocity) 			{ velocity = mVelocity; }
	const vector<string>& Body::getGroups()				{ return groups; }
	const vector<string>& Body::getGroupsToCheck()		{ return groupsToCheck; }
	Vector2i Body::getPosition() 						{ return position; }
	Vector2i Body::getVelocity() 						{ return velocity; }
	void* Body::getUserData()							{ return userData; }
	int Body::getX() 									{ return position.x; }
	int Body::getY() 									{ return position.y; }
	int Body::getLeft() 								{ return position.x - halfSize.x; }
	int Body::getRight() 								{ return position.x + halfSize.x; }
	int Body::getTop() 									{ return position.y - halfSize.y; }
	int Body::getBottom() 								{ return position.y + halfSize.y; }
	int Body::getOldX() 								{ return oldPosition.x; }
	int Body::getOldY() 								{ return oldPosition.y; }
	int Body::getOldLeft() 								{ return oldPosition.x - halfSize.x; }
	int Body::getOldRight() 							{ return oldPosition.x + halfSize.x; }
	int Body::getOldTop() 								{ return oldPosition.y - halfSize.y; }
	int Body::getOldBottom() 							{ return oldPosition.y + halfSize.y; }
	int Body::getHalfWidth() 							{ return halfSize.x; }
	int Body::getHalfHeight() 							{ return halfSize.y; }
	int Body::getWidth() 								{ return halfSize.x * 2; }
	int Body::getHeight() 								{ return halfSize.y * 2; }
}
