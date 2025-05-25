#pragma once
#include <SFML/Graphics.hpp>
#include "Bonus.h"



class Brick {
public:
    explicit Brick(const sf::Vector2f& position, bool indestructible = false);
    void draw(sf::RenderWindow& window) const;
    sf::RectangleShape shape;
    int health = 3;

    bool isIndestructible = false;
    bool hasBonus = false;
    BonusType bonusType = BonusType::EnlargePaddle;

};