#pragma once
#include "Game.h"

class Text
{
public:
	Text(std::string content, float x, float y, Game* game);
	void draw();
	std::string content; //texto
	int x;
	int y;
	int width;
	int height;
	Game* game; // Referencia al juego
};

