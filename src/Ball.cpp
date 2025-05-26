#include "Ball.h"
#include <cmath>

Ball::Ball(float x, float y) {
    shape.setRadius(10.f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(sf::Vector2f(x, y));
    velocity = {speed, speed};


    indicator = sf::VertexArray(sf::PrimitiveType::Lines, 2);
    indicator[0].color = sf::Color::White;
    indicator[1].color = sf::Color::Transparent;
}

void Ball::setDirectionFromMouse(const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f ballCenter(
        shape.getPosition().x + shape.getRadius(),
        shape.getPosition().y + shape.getRadius()
    );

    sf::Vector2f direction(mousePos.x - ballCenter.x, mousePos.y - ballCenter.y);


    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0.f) {
        direction /= length;
    }

    velocity = direction * speed;
    directionSet = true;


    indicator[0].position = ballCenter;
    indicator[1].position = ballCenter + direction * 100.f;
}

void Ball::update(const Paddle& paddle) {
    if (!isLaunched) {

        shape.setPosition(sf::Vector2f(
    paddle.shape.getPosition().x + paddle.shape.getSize().x / 2 - shape.getRadius(),
    paddle.shape.getPosition().y - shape.getRadius()
));
        return;
    }

    shape.move(velocity);


    if (shape.getPosition().x <= 0 || shape.getPosition().x + shape.getRadius() * 2 >= 730)
        velocity.x *= -1;

    if (shape.getPosition().y <= 0)
        velocity.y *= -1;


    if (shape.getPosition().y > 730) {
        isLaunched = false;
        reset(paddle);
    }
}


void Ball::draw(sf::RenderWindow& window) const {
    window.draw(shape);

    if (!isLaunched && directionSet) {
        window.draw(indicator);
    }
}

void Ball::reset(const Paddle& paddle) {
    shape.setPosition(sf::Vector2f(
    paddle.shape.getPosition().x + paddle.shape.getSize().x / 2 - shape.getRadius(),
    paddle.shape.getPosition().y - shape.getRadius()
));
    velocity = {speed, speed};
    directionSet = false;

}


void Ball::accelerate(float factor) {
    velocity *= factor;

    const float maxSpeed = 6.f;
    velocity.x = std::clamp(velocity.x, -maxSpeed, maxSpeed);
    velocity.y = std::clamp(velocity.y, -maxSpeed, maxSpeed);
}