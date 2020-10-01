#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Audio.h"

#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void checkColisionEnemyShoot(Enemy* enemy, std::list<Enemy*> &deleteEnemies, std::list<Projectile*> &deleteProjectiles);
	void draw() override;
	void keysToControls(SDL_Event event);

	Player* player;
	Background* background;
	Actor* backgroundPoints;
	
	Audio* audioBackground;

	Text* textPoints;
	int points;

	bool controlShoot = false;
	int controlMoveX = 0;
	int controlMoveY = 0;
	int newEnemyTime = 0;

	std::list<Enemy*> enemies;
	std::list<Projectile*> projectiles;
};

