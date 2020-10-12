#pragma once

#include "Actor.h"

class Door : public Actor
{
public:
	Door(int number, float x, float y, Game* game);

	int number;
	Door* doorConnected;
};

