#pragma once

#include "Actor.h"
#include "Animation.h"

class Collectable : public Actor
{
public:
	Collectable(float x, float y, Game* game);

	Animation* animation;
	void update();
	void draw(float scrollX = 0, float scrollY = 0) override;
};

