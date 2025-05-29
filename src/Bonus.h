#pragma once

#include <SFML/Graphics.hpp>
#include <memory>


class Paddle;
class Ball;
class Game;


class Bonus {
public:
    virtual ~Bonus() = default;

    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    virtual void applyEffect(Paddle& paddle, Ball& ball, Game& game) = 0;

    bool isCollected() const { return collected; }
    void collect() { collected = true; }

    sf::CircleShape getShape() const { return shape; }

protected:
    sf::CircleShape shape;
    bool collected = false;
};

using BonusPtr = std::unique_ptr<Bonus>;


class BigPaddle : public Bonus {
public:
    explicit BigPaddle(const sf::Vector2f& position);
    void update() override;
    void draw(sf::RenderWindow& window) const override;
    void applyEffect(Paddle& paddle, Ball& ball, Game& game ) override;
};


class FastBall : public Bonus {
public:
    explicit FastBall(const sf::Vector2f& position);
    void update() override;
    void draw(sf::RenderWindow& window) const override;
    void applyEffect(Paddle& paddle, Ball& ball, Game& game ) override;
};


class BottomPlatform : public Bonus {
public:
    explicit BottomPlatform(const sf::Vector2f& position);
    void update() override;
    void draw(sf::RenderWindow& window) const override;
    void applyEffect(Paddle& paddle, Ball& ball, Game& game ) override;
};

class RandomTrajectory : public Bonus {
public:
    explicit RandomTrajectory(const sf::Vector2f& position);
    void update() override;
    void draw(sf::RenderWindow& window) const override;
    void applyEffect(Paddle& paddle, Ball& ball, Game& game ) override;
};