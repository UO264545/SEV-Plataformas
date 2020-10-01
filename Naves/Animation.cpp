#include "Animation.h"

Animation::Animation(std::string filename, float actorWidth, float actorHeight,
	float fileWidth, float fileHeight,
	int updateFrecuence, int totalFrames, bool loop, Game* game) {

	// Cargar textura
	texture = game->getTexture(filename);

	this->loop = loop;

	this->actorWidth = actorWidth;
	this->actorHeight = actorHeight;
	this->fileWidth = fileWidth;
	this->fileHeight = fileHeight;
	this->updateFrecuence = updateFrecuence;
	this->totalFrames = totalFrames;
	this->game = game;

	updateTime = 0; // �ltima actualizaci�n
	currentFrame = 0;

	// Calcular lo que mide un fotograma/frame
	frameWidth = fileWidth / totalFrames;
	frameHeigt = fileHeight;

	// Rectangulo de recorte de fotograma
	source.x = 0;
	source.y = 0;
	source.w = frameWidth;
	source.h = frameHeigt;
}

bool Animation::update() {
	updateTime++;
	if (updateTime > updateFrecuence) {
		updateTime = 0;
		// Actualizar el frame
		currentFrame++;
		// Si lleva al �ltimo frame vuelve al primero
		if (currentFrame >= totalFrames) {
			// Reiniciar es infinita
			currentFrame = 0;

			if (!loop)
				return true;
		}
	}
	// Actualizar el rect�ngulo del source (siguiente frame)
	source.x = currentFrame * frameWidth;
	return false; //luego lo cambiamos
}

void Animation::draw(float x, float y) {
	// Donde se va a pegar en el renderizador
	SDL_Rect destination;
	destination.x = x - actorWidth / 2;
	destination.y = y - actorHeight / 2;
	destination.w = actorWidth;
	destination.h = actorHeight;

	// Modificar para quue la referencia sea el punto central
	SDL_RenderCopyEx(game->renderer, texture, &source, &destination, 0, nullptr, SDL_FLIP_NONE);
}