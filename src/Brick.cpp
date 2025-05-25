#include "Brick.h"

Brick::Brick(const sf::Vector2f& position, bool indestructible)
    : isIndestructible(indestructible)
{
    shape.setSize({80, 20});
    shape.setPosition(position);

    if (indestructible) {
        shape.setFillColor(sf::Color::Blue);
    } else {
        shape.setFillColor(sf::Color::Green);
        hasBonus = (rand() % 100) < 80;

        if (hasBonus) {
            int bonusRoll = rand() % 4;

            switch (bonusRoll) {
                case 0:
                    bonusType = BonusType::EnlargePaddle;
                break;
                case 1:
                    bonusType = BonusType::SpeedUpBall;
                break;
                case 2:
                    bonusType = BonusType::BottomPass;
                break;
                case 3:
                    bonusType = BonusType::RandomTrajectory;
                break;
            }
        }
    }
}


void Brick::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}