#include "Game.h"
#undef main //esto es para quitar el main que hay en la Librer�a SDL_main
int main() {
	Game* game = new Game();
	//return 0; En el main no es necesario devolver nada
}