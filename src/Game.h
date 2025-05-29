#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "Bonus.h"

class Game {
public:
    Game();
    void run();

    void activateBottomPlatform();
    void activateRandomTrajectory();


private:
    void initBricks();
    void processEvents();
    void update();
    void render();



    sf::RenderWindow window;
    Paddle paddle;
    Ball ball;

    std::vector<std::unique_ptr<Brick>> bricks;
    std::vector<std::unique_ptr<Bonus>> bonuses;

    bool hasBottomPlatform = false;
    sf::RectangleShape bottomPlatform;

    bool hasRandomTrajectory = false;
    sf::Clock randomTrajTimer;
};