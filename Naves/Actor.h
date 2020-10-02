#pragma once
#include "Game.h"

class Actor
{
public:
	Actor(std::string filename, float x, float y, int width, int height, Game* game);
	~Actor();
	virtual void draw(float scrollX = 0);
	bool isInRender(float scrollX = 0);
	bool isOverlap(Actor* actor);

	bool collisionDown;

	SDL_Texture* texture;
	float x; //posicion en x
	float y;
	float vx; //velocidad en x
	float vy;
	int width;
	int height;
	int fileWidth;
	int fileHeight;
	Game* game; // referencia al juego
};

