#include "Tile.h"
#include <cstdlib>
#include "GameLogic.h"


void Tile::spawn(int gx, int gy) {
    x = gx >= 0 && gx < BOARD_SIZE ? gx : 0;
    y = gy >= 0 && gy < BOARD_SIZE ? gy : 0;
    color = rand() % 6;
}