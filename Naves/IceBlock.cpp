#include "IceBlock.h"

IceBlock::IceBlock(float x, float y, Game* game) 
	: DestructibleBlock("res/bloque_hielo.png", 40, 32, x, y, game) {
	animationBreak = new Animation("res/bloque_hielo_roto.png", 40, 32, 233, 32, 1, 6, false, game);
}

void IceBlock::checkCollision(Actor* actor) {
	int actorDown = actor->y + actor->height / 2;

	int blockTop = y - height / 2;
	int blockRight = x + width / 2;
	int blockLeft = x - width / 2;

	if ((blockTop - 5) <= actorDown
		&& blockRight >= actor->x
		&& blockLeft <= actor->x)
		collide();
}
