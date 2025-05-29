#include "Brick.h"
#include <cstdlib>

Brick::Brick(const sf::Vector2f& position, bool indestructible)
    : isIndestructible(indestructible)
{
    shape.setSize({80, 20});
    shape.setPosition(position);

    if (indestructible) {
        shape.setFillColor(sf::Color::Blue);
    } else {
        shape.setFillColor(sf::Color::Green);
        health = 3;
        hasBonus = (rand() % 100) < 80; // 80% шанс получить бонус

        if (hasBonus) {
            int bonusRoll = rand() % 4;

            switch (bonusRoll) {
                case 0:
                    bonus = std::make_unique<BigPaddle>(position);
                break;
                case 1:
                    bonus = std::make_unique<FastBall>(position);
                break;
                case 2:
                    bonus = std::make_unique<BottomPlatform>(position);
                break;
                case 3:
                    bonus = std::make_unique<RandomTrajectory>(position);
                break;
            }
        }
    }
}

void Brick::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect Brick::getBounds() const {
    return shape.getGlobalBounds();
}

bool Brick::isDestroyed() const {
    return !isIndestructible && health <= 0;
}

int Brick::getHealth() const {
    return health;
}

void Brick::hit() {
    if (!isIndestructible) {
        --health;
        if (health == 2) {
            shape.setFillColor(sf::Color::Yellow);
        } else if (health == 1) {
            shape.setFillColor(sf::Color::Red);
        } else {
            shape.setFillColor(sf::Color::Black);
        }
    }
}

std::unique_ptr<Bonus> Brick::releaseBonus() {
    return std::move(bonus);
}