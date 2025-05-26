#include "Bonus.h"
#include <cmath>

Bonus::Bonus(const sf::Vector2f& position, BonusType type_)
    : type(type_), collected(false)
{
    shape.setRadius(10.f);
    shape.setPosition(position);

    switch (type) {
        case BonusType::EnlargePaddle:
            shape.setFillColor(sf::Color::Blue);
        break;
        case BonusType::SpeedUpBall:
            shape.setFillColor(sf::Color::Magenta);
        break;
        case BonusType::BottomPass:
            shape.setFillColor(sf::Color::Green);
        break;
        case BonusType::RandomTrajectory:
            shape.setFillColor(sf::Color::Yellow);
        break;

    }
}

void Bonus::update() {
    float fallSpeed = 0.1f;

    if (type == BonusType::EnlargePaddle) {
        fallSpeed = 0.1f;
    } else if (type == BonusType::SpeedUpBall) {
        fallSpeed = 0.1f;
    }  else if (type == BonusType::BottomPass) {
        fallSpeed = 0.1f;
    }else if (type == BonusType::RandomTrajectory) {
        fallSpeed = 0.1f;
    }


    shape.move(sf::Vector2f(0.f, fallSpeed));

    if (shape.getPosition().y > 730) {
        collected = true;
    }
}

void Bonus::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

bool Bonus::isCollected() const {
    return collected;
}

void Bonus::collect() {
    collected = true;
}