#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Audio.h"
#include "Tile.h"
#include "Space.h"
#include "Pad.h"

#include <list>
#include <fstream> // Leer ficheros
#include <sstream> // Leer lineas / string

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void checkColisionShoot(std::list<Enemy*> &deleteEnemies, std::list<Projectile*> &deleteProjectiles);
	void draw() override;
	void keysToControls(SDL_Event event);

	Player* player;
	Background* background;
	Actor* backgroundPoints;
	
	Actor* backgroundLifes[Player::INITIAL_LIFES];

	Audio* audioBackground;

	Text* textPoints;
	int points;

	void loadMap(std::string name);
	void loadMapObject(char character, float x, float y);
	int mapWidth;
	list<Tile*> tiles;

	Tile* cup;

	void calculateScroll();
	float scrollX;

	Space* space;

	bool controlShoot = false;
	int controlMoveX = 0;
	int controlMoveY = 0;
	int newEnemyTime = 0;

	std::list<Enemy*> enemies;
	std::list<Projectile*> projectiles;

	// Elementos de interfaz
	Pad* pad;
	Actor* buttonJump;
	Actor* buttonShoot;

	Actor* message;
	bool pause;

	bool controlContinue = false;

	void mouseToControls(SDL_Event event); //USO DE MOUSE
};

