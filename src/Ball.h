#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"

class Ball {
public:
    static constexpr float speed = 0.2f;

    Ball(float x, float y);

    void update(const Paddle& paddle);
    void draw(sf::RenderWindow& window) const;

    void reset(const Paddle& paddle);
    void accelerate(float factor = 0.2f);
    void setDirectionFromMouse(const sf::RenderWindow& window);
    void setVelocity(sf::Vector2f newVelocity);

    // Геттеры
    sf::CircleShape getShape() const { return shape; }
    sf::Vector2f getVelocity() const { return velocity; }


    bool isLaunched = false;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;

    sf::VertexArray indicator;
    bool directionSet = false;
};