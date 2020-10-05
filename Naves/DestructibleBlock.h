#pragma once

#include "Actor.h"
#include "Animation.h"

class DestructibleBlock : public Actor
{
public:
	DestructibleBlock(float x, float y, Game* game);

	int state = game->stateIdle;

	void collide();

	void update();
	void draw(float scrollX = 0) override;

	Animation* animationBreak;
};

