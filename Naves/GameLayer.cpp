#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
		//llama al constructor del padre : Layer(renderer)
	init();
}

void GameLayer::init() {
	space = new Space(1);
	scrollX = 0;
	tiles.clear();

	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	points = 0;
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = std::to_string(points);

	delete player; //borra el jugador anterior
	//player = new Player(50, 50, game); Se crea cuando carga el mapa
	background = new Background("res/fondo_2.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png", WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);

	projectiles.clear(); // Vaciar por si reiniciamos el juego

	enemies.clear(); // Vaciar por si reiniciamos el juego
	//enemies.push_back(new Enemy(300, 50, game));
	//enemies.push_back(new Enemy(300, 200, game));

	loadMap("res/0.txt");
}

void GameLayer::processControls() {
	//obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}

	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
		}
	}
	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}
	// Eje Y
	if (controlMoveY < 0) {
		player->jump();
	}
	
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_QUIT) {
		game->loopActive = false;
	}

	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_1:
			game->scale();
			break;
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}
	}
}

void GameLayer::update() {
	using namespace std;

	if (player->y > HEIGHT + player->height) // cuando el personaje cae del todo
		init();

	space->update();
	background->update();
	// Generar enemigos
	/*newEnemyTime--;
	if (newEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Enemy(rX, rY, game));
		newEnemyTime = 110;
	}*/

	player->update();
	for (auto const& enemy : enemies) {
		enemy->update();
	}
	for (auto const& projectile : projectiles) {
		projectile->update();
	}
	for (auto const& block : destructibleBlocks) {
		block->update();
	}

	// Colisiones
	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	list<DestructibleBlock*> deleteBlocks;
	
	for (auto const& enemy : enemies) {
		// Eliminar enemigos que salen por la izquierda
		if (enemy->x < 0)
			deleteEnemies.push_back(enemy);
		if (player->isOverlap(enemy)) {
			player->loseLife();
			if (player->lifes <= 0) {
				init();
				return; // Cortar el for
			}
		}
	}

	checkColisionProjectile(deleteEnemies, deleteProjectiles);

	for (auto const& enemy : enemies) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();
			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
		}
	}

	for (auto const& block : destructibleBlocks) {
		if (block->state == game->stateDead) {
			bool bInList = std::find(deleteBlocks.begin(),
				deleteBlocks.end(),
				block) != deleteBlocks.end();
			if (!bInList) {
				deleteBlocks.push_back(block);
			}
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();

	for (auto const& delBlock : deleteBlocks) {
		destructibleBlocks.remove(delBlock);
		space->removeStaticActor(delBlock);
	}
	deleteProjectiles.clear();

	// cout << "update GameLayer" << endl;
}

// Colisi�n Enemy - Shoot
void GameLayer::checkColisionProjectile(std::list<Enemy*> &deleteEnemies, std::list<Projectile*> &deleteProjectiles) {
	for (auto const& projectile : projectiles) {
		// Eliminar proyectiles que salen por la derecha
		if (!projectile->isInRender(scrollX) || projectile->vx == 0) {
			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();
			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
		for (auto const& enemy : enemies) {
			if (enemy->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();
				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
				enemy->impacted();
				points++;
				textPoints->content = std::to_string(points);
			}
		}
		for (auto const& block : destructibleBlocks) {
			if (projectile->isOverlap(block)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();
				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
				block->collide();
			}
		}
	}
}

void GameLayer::draw() {
	calculateScroll();

	//primero el background y despu�s el player, sino no se ve el player
	background->draw();
	for (auto const& tile : tiles)
		tile->draw(scrollX);

	player->draw(scrollX);

	for (auto const& enemy : enemies) {
		enemy->draw(scrollX);
	}

	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX);
	}

	for (auto const& block : destructibleBlocks) {
		block->draw(scrollX);
	}

	textPoints->draw();
	backgroundPoints->draw(); //asi no lo tapa un enemigo

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str()); // ifstream streamFile = new ifstream(name.c_str()); es lo mismo
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por l�nea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por car�cter (en cada l�nea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}
			cout << character << endl;
		}
	}
	streamFile.close();
}

void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
	case 'D': {
		DestructibleBlock* block = new DestructibleBlock(x, y, game);
		block->y = block->y - block->height / 2;
		destructibleBlocks.push_back(block);
		space->addStaticActor(block);
		break;
	}
	case 'E': {
		Enemy* enemy = new Enemy(x, y, game);
		// modificaci�n para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		break;
	}
	case '1': {
		player = new Player(x, y, game);
		// modificaci�n para empezar a contar desde el suelo.
		player->y = player->y - player->height / 2;
		space->addDynamicActor(player);
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/bloque_tierra.png", x, y, game);
		// modificaci�n para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	}
}

void GameLayer::calculateScroll() {
	// limite izquierda
	if (player->x > WIDTH * 0.3) {// Dentro del l�mite del mapa
		if (player->x - scrollX < WIDTH * 0.3) {
			scrollX = player->x - WIDTH * 0.3;
		}
	}

	// limite derecha
	if (player->x < mapWidth - WIDTH * 0.3) { // Dentro del l�mite del mapa
		if (player->x - scrollX > WIDTH * 0.7) {
			scrollX = player->x - WIDTH * 0.7;
		}
	}
}
