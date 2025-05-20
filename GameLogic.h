#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include "Tile.h"
#include <iostream>
#include <cstdlib>
#include <random>
#include <utility>
#include <algorithm>

constexpr unsigned int BOARD_SIZE = 8;
constexpr unsigned int TILE_SIZE = 80;
constexpr unsigned int WINDOW_WIDTH = 640;
constexpr unsigned int WINDOW_HEIGHT = 640;

class GameLogic {
public:
    GameLogic();
    void run();

private:
    std::vector<std::vector<Tile>> board;
    sf::RenderWindow window;
    static inline std::mt19937 globalRng = std::mt19937(std::random_device{}());


    struct Bonus {
        int type;
        int x, y;
        bool active = true;
    };

    std::vector<Bonus> bonuses;


    sf::Texture textures[6];
    sf::Texture bonusTexture;
    sf::Texture bombTexture;

    sf::Vector2i selected{-1, -1};
    bool matchFound = true;


    std::vector<std::vector<bool>> matched;


    void initBoard();
    void render();
    void handleInput();
    void findMatches();
    void collapseTiles();
    void processBonus(const Bonus& bonus);
    void processBomb(const Bonus& bomb);

};