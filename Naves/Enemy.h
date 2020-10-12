#pragma once

#include "Actor.h"
#include "Animation.h"

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void update();
	void draw(float scrollX = 0, float scrollY = 0) override; // Sobreescribe

	void impacted(); // Recibe impacto y pone animación de morir
	int state;

	float vxIntelligence;

	Animation* aMoving;
	Animation* aDying;
	Animation* animation; // Referencia a la animación actual
};


