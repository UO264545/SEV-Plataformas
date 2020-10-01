#pragma once

#include "Actor.h"
#include "Animation.h"

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void update();
	void draw() override; // Sobreescribe

	void impacted(); // Recibe impacto y pone animación de morir
	int state;

	Animation* aMoving;
	Animation* aDying;
	Animation* animation; // Referencia a la animación actual
};


