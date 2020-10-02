#include "Actor.h"

#include <list>

class Space
{
public:
	Space(float gravity);
	void update();
	void updateMoveRight(Actor* dynamicActor);
	void updateMoveLeft(Actor* dynamicActor);
	void updateMoveTop(Actor* dynamicActor);
	void updateMoveDown(Actor* dynamicActor);
	void addDynamicActor(Actor* actor);
	void addStaticActor(Actor* actor);
	void removeDynamicActor(Actor* actor);
	void removeStaticActor(Actor* actor);
	float gravity;
	std::list<Actor*> dynamicActors;
	std::list<Actor*> staticActors;
};

