#include "Tile.h"

Tile::Tile(std::string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 40, 32, game) {
}
