#pragma once

struct Tile {
    int color = -1;
    bool destroyed = false;
    int x = 0, y = 0;

    void spawn(int gx, int gy);
};