#pragma once
#include <SFML/Graphics.hpp>

enum class BonusType {
    EnlargePaddle,
    SpeedUpBall,
    BottomPass,
    RandomTrajectory,
};

class Bonus {
public:
    Bonus(const sf::Vector2f& position, BonusType type);
    void update();
    void draw(sf::RenderWindow& window) const;
    bool isCollected() const;
    void collect();

    sf::CircleShape shape;
    BonusType type;

private:
    bool collected = false;
};