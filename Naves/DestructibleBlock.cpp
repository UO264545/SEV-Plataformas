#include "DestructibleBlock.h"

DestructibleBlock::DestructibleBlock(float x, float y, Game* game)
	: Actor("res/destructible.png", x, y, 32, 21, game) {
	animationBreak = new Animation("res/destructible_roto.png", 32, 32, 128, 32, 3, 4, false, game);
}

void DestructibleBlock::collide() {
	this->state = game->stateDying;
}

void DestructibleBlock::update() {
	if (state == game->stateDying) {
		if (animationBreak->update())
			state = game->stateDead;
	}
}

void DestructibleBlock::draw(float scrollX) {
	if (state == game->stateDying)
		animationBreak->draw(x - scrollX, y);
	else
		Actor::draw(scrollX);
}