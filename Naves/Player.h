#pragma once

#include "Actor.h"
#include "Projectile.h"
#include "Audio.h"
#include "Animation.h" // incluir animacion 

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	Projectile* shoot();
	void update();
	void moveX(float axis);
	void jump();

	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	
	void loseLife();
	static int const INITIAL_LIFES = 3;
	int lifes = INITIAL_LIFES;
	int invulnerableTime = 0;
	
	int orientation;
	int state;

	bool onAir;

	Animation* aIdleRight;
	Animation* aIdleLeft;
	Animation* aRunningRight;
	Animation* aRunningLeft;

	Animation* aJumpingRight;
	Animation* aJumpingLeft;

	Animation* aShootingRight;
	Animation* aShootingLeft;

	Animation* animation; // Referencia a la animación mostrada

	Audio* audioShoot;
	int shootCadence = 30;
	int shootTime = 0;
};

