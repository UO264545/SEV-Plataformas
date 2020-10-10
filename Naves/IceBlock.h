#pragma once

#include "DestructibleBlock.h"

class IceBlock : public DestructibleBlock
{
public:
	IceBlock(float x, float y, Game* game);
	void checkCollision(Actor* actor);
};

