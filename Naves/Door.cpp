#include "Door.h"

Door::Door(int number, float x, float y, Game* game)
	: Actor("res/puerta.png", x, y, 46, 58, game) {
	this->number = number;
}
