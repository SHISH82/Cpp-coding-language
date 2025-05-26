#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"

class Ball {
public:
    static constexpr float speed = 0.1f;
    Ball(float x, float y);
    void update(const Paddle& paddle);
    void draw(sf::RenderWindow& window) const;
    void reset(const Paddle& paddle);
    void accelerate(float factor = 0.2f);
    void setDirectionFromMouse(const sf::RenderWindow& window);


    bool isLaunched = false;



    sf::CircleShape shape;
    sf::Vector2f velocity;

private:
    sf::VertexArray indicator;
    bool directionSet = false;
};
