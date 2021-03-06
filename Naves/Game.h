#pragma once
// Todas las librerias de SDL y otras utilidades
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <map>

#include "Layer.h"

// Valores generales
#define WIDTH 480
#define HEIGHT 320

#include "Layer.h"
class Layer;

enum class GameInputType {
	KEYBOARD, GAMEPAD, MOUSE
};

class Game
{
public:
	Game();
	void loop();
	void scale();
	SDL_Texture* getTexture(std::string filename);
	std::map<std::string, SDL_Texture*> mapTextures; // map - cache
	TTF_Font* font;
	bool scaledToMax = false;
	float scaleLower = 1;
	SDL_Window* window; // ventana
	SDL_Renderer* renderer; // renderizador
	bool loopActive; // Juego activo

	Layer* gameLayer;
	Layer* layer; // ventana activa
	Layer* menuLayer;

	int const stateMoving = 1;
	int const stateJumping = 2;
	int const stateDying = 3;
	int const stateDead = 4;
	int const stateShooting = 5;

	int const orientationRight = 1;
	int const orientationLeft = 2;

	int currentLevel = 0;
	int finalLevel = 2;

	GameInputType input;
};

