#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Bonus.h"

class Brick {
public:
    explicit Brick(const sf::Vector2f& position, bool indestructible = false);

    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;
    bool isDestroyed() const;

    bool isIndestructible = false;
    bool hasBonus = false;

    int getHealth() const;
    void hit();


    std::unique_ptr<Bonus> releaseBonus();

private:
    sf::RectangleShape shape;
    int health = 3;

    std::unique_ptr<Bonus> bonus = nullptr;
};