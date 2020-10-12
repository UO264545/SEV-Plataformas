#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
		//llama al constructor del padre : Layer(renderer)
	pause = true;
	message = new Actor("res/mensaje_como_jugar.png", WIDTH * 0.5, HEIGHT * 0.5,
		WIDTH, HEIGHT, game);
	init();
}

void GameLayer::init() {
	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.8, game);
	buttonJump = new Actor("res/boton_salto.png", WIDTH * 0.9, HEIGHT * 0.55, 100, 100, game);
	buttonShoot = new Actor("res/boton_disparo.png", WIDTH * 0.75, HEIGHT * 0.83, 100, 100, game);

	space = new Space(1);
	scrollX = 0;
	scrollY = 0;
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

	destructibleBlocks.clear();

	loadMap("res/" + std::to_string(game->currentLevel) + ".txt");

	for (int i = 0; i < Player::INITIAL_LIFES; i++) {
		backgroundLifes[i] = new Actor("res/icono_vidas.png", WIDTH * 0.1 + (i * WIDTH * 0.075), HEIGHT * 0.11, 40, 40, game);
	}
}

void GameLayer::processControls() {
	//obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			game->loopActive = false;
			return;
		}
		// Cambio autom�tico de input
		if (event.type == SDL_KEYDOWN) {
			game->input = GameInputType::KEYBOARD;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = GameInputType::MOUSE;
		}
		// Procesar teclas
		if(game->input == GameInputType::KEYBOARD)
			keysToControls(event);
		else if(game->input == GameInputType::MOUSE)
			mouseToControls(event);
	}

	if (controlContinue) {
		pause = false;
		controlContinue = false;
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

void GameLayer::mouseToControls(SDL_Event event) {
	// Modificaci�n de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;
	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		controlContinue = true;
		if (pad->containsPoint(motionX, motionY)) {
			controlMoveX = pad->getOrientationX(motionX);
			pad->clicked = true;
		}
		else {
			controlMoveX = 0;
			pad->clicked = false;
		}
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = true;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = -1;
		}
	}
	// Cada vez que se mueve
	if (event.type == SDL_MOUSEMOTION) {
		if (pad->clicked 
			&& pad->containsPoint(motionX, motionY)) {
			controlMoveX = pad->getOrientationX(motionX);
			// Rango de -20 a 20 es igual que 0
			if (controlMoveX > -20 && controlMoveX < 20)
				controlMoveX = 0;
		} else {
			controlMoveX = 0;
		}
		if (!buttonShoot->containsPoint(motionX, motionY))
			controlShoot = false;
		if (!buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = 0;
		}
	}
	// Cada vez que levantan el click
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = false;
			controlMoveX = 0;
		}
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = 0;
		}
	}
}

void GameLayer::update() {
	using namespace std;

	if (pause) {
		return;
	}

	// Nivel superado
	if (cup->isOverlap(player)) {
		game->currentLevel++;
		if (game->currentLevel > game->finalLevel) {
			game->currentLevel = 0;
		}
		message = new Actor("res/mensaje_ganar.png", WIDTH * 0.5, HEIGHT * 0.5,
			WIDTH, HEIGHT, game);
		pause = true;
		init();
	}

	if (player->y > HEIGHT + player->height + scrollY) {// cuando el personaje cae del todo
		message = new Actor("res/mensaje_perder.png", WIDTH * 0.5, HEIGHT * 0.5,
			WIDTH, HEIGHT, game);
		pause = true;
		init();
	}

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

	for (auto const& block : iceBlocks) {
		block->update();
	}

	// Colisiones
	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	list<DestructibleBlock*> deleteBlocks;
	list<IceBlock*> deleteIceBlocks;
	
	// Comprobar que el jugador est� sobre los bloques de hielo
	for (auto const& block : iceBlocks) {
		block->checkCollision(player);
	}

	for (auto const& enemy : enemies) {
		// Eliminar enemigos que salen por la izquierda
		if (enemy->x < 0)
			deleteEnemies.push_back(enemy);
		if (player->isOverlap(enemy)) {
			player->loseLife();
			if (player->lifes <= 0) {
				message = new Actor("res/mensaje_perder.png", WIDTH * 0.5, HEIGHT * 0.5,
					WIDTH, HEIGHT, game);
				pause = true;
				init();
				return; // Cortar el for
			}
		}
	}

	checkColisionShoot(deleteEnemies, deleteProjectiles);

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

	for (auto const& block : iceBlocks) {
		if (block->state == game->stateDead) {
			bool bInList = std::find(deleteIceBlocks.begin(),
				deleteIceBlocks.end(),
				block) != deleteIceBlocks.end();
			if (!bInList) {
				deleteIceBlocks.push_back(block);
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
	deleteBlocks.clear();

	for (auto const& delBlock : deleteIceBlocks) {
		iceBlocks.remove(delBlock);
		space->removeStaticActor(delBlock);
	}
	deleteIceBlocks.clear();
}

// Colisi�n Enemy - Shoot
void GameLayer::checkColisionShoot(std::list<Enemy*> &deleteEnemies, std::list<Projectile*> &deleteProjectiles) {
	for (auto const& projectile : projectiles) {
		// Eliminar proyectiles que salen por la derecha
		if (!projectile->isInRender(scrollX, scrollY) || projectile->vx == 0) {
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
		tile->draw(scrollX, scrollY);

	cup->draw(scrollX, scrollY);

	player->draw(scrollX, scrollY);

	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}

	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX, scrollY);
	}

	for (auto const& block : destructibleBlocks) {
		block->draw(scrollX, scrollY);
	}

	for (auto const& block : iceBlocks) {
		block->draw(scrollX, scrollY);
	}

	// HUD
	textPoints->draw();
	backgroundPoints->draw(); //asi no lo tapa un enemigo
	for (int i = 0; i < player->lifes; i++) {
		backgroundLifes[i]->draw();
	}
	if (game->input == GameInputType::MOUSE) {
		buttonJump->draw(); // NO TIENEN SCROLL, POSICI�N FIJA
		buttonShoot->draw();
		pad->draw();
	}

	if (pause) {
		message->draw();
	}

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
	case 'W': {
		IceBlock* block = new IceBlock(x, y, game);
		block->y = block->y - block->height / 2;
		iceBlocks.push_back(block);
		space->addStaticActor(block);
		break;
	}
	case 'U': {
		DestructibleBlock* block = new DestructibleBlock(x, y, game);
		block->y = block->y - block->height / 2;
		destructibleBlocks.push_back(block);
		space->addStaticActor(block);
		break;
	}
	case 'C': {
		cup = new Tile("res/copa.png", x, y, game);
		// modificaci�n para empezar a contar desde el suelo.
		cup->y = cup->y - cup->height / 2;
		space->addDynamicActor(cup);
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

	// limite arriba
	if (player->y > HEIGHT * 0.3) {// Dentro del l�mite del mapa
		if (player->y - scrollY < HEIGHT * 0.3) {
			scrollY = player->y - HEIGHT * 0.3;
		}
	}

	// limite abajo
	if (player->y < mapWidth - HEIGHT * 0.3) { // Dentro del l�mite del mapa
		if (player->y - scrollY > HEIGHT * 0.7) {
			scrollY = player->y - HEIGHT * 0.7;
		}
	}
}
