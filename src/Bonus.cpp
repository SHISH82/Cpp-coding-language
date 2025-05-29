#include "Bonus.h"
#include "Paddle.h"
#include "Ball.h"
#include "Game.h"
#include <cmath>
#include <cstdlib>




BigPaddle::BigPaddle(const sf::Vector2f& position) {
    shape.setRadius(10.f);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Blue);
}

void BigPaddle::update() {
    constexpr float fallSpeed = 0.1f;
    shape.move({0.f, fallSpeed});
    if (shape.getPosition().y > 730)
        collected = true;
}

void BigPaddle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

void BigPaddle::applyEffect(Paddle& paddle, Ball& ball, Game& game ) {
    paddle.changeSize(paddle.getWidth() * 1.1f);
}



FastBall::FastBall(const sf::Vector2f& position) {
    shape.setRadius(10.f);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Magenta);
}

void FastBall::update() {
    constexpr float fallSpeed = 0.1f;
    shape.move({0.f, fallSpeed});
    if (shape.getPosition().y > 730)
        collected = true;
}

void FastBall::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

void FastBall::applyEffect(Paddle& paddle, Ball& ball, Game& game ) {
    ball.accelerate(1.1f);
}



BottomPlatform::BottomPlatform(const sf::Vector2f& position) {
    shape.setRadius(10.f);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Green);
}

void BottomPlatform::update() {
    constexpr float fallSpeed = 0.1f;
    shape.move({0.f, fallSpeed});
    if (shape.getPosition().y > 730)
        collected = true;
}

void BottomPlatform::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

void BottomPlatform::applyEffect(Paddle& paddle, Ball& ball, Game& game ) {

    game.activateBottomPlatform();

}



RandomTrajectory::RandomTrajectory(const sf::Vector2f& position) {
    shape.setRadius(10.f);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::Yellow);
}

void RandomTrajectory::update() {
    constexpr float fallSpeed = 0.1f;
    shape.move({0.f, fallSpeed});
    if (shape.getPosition().y > 730)
        collected = true;
}

void RandomTrajectory::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

void RandomTrajectory::applyEffect(Paddle& paddle, Ball& ball, Game& game ) {
    float randomAngle = static_cast<float>(rand() % 360 - 180);
    float angleRad = randomAngle * M_PI / 180.f;
    float currentSpeed = std::sqrt(ball.getVelocity().x * ball.getVelocity().x + ball.getVelocity().y * ball.getVelocity().y);

    ball.setVelocity({
        currentSpeed * cos(angleRad),
        currentSpeed * sin(angleRad)
    });
    game.activateRandomTrajectory();
}