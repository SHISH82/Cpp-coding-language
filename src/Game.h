#pragma once
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

private:
    void initBricks();
    void processEvents();
    void update();
    void render();

    sf::RenderWindow window;
    Paddle paddle;
    Ball ball;
    std::vector<Brick> bricks;
    std::vector<Bonus> bonuses;

    bool hasBottomPlatform = false;
    sf::RectangleShape bottomPlatform;

    bool hasRandomTrajectory = false;
    sf::Clock randomTrajTimer;
    float randomTrajDuration = 2.0f;


};
