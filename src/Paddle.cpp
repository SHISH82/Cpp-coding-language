#include "Paddle.h"

Paddle::Paddle(float x, float y, float initialWidth)
    : shape(sf::Vector2f(initialWidth, 20.f))
{
    shape.setFillColor(sf::Color::White);
    shape.setPosition(sf::Vector2f(x, y));
}

void Paddle::update() {
}

void Paddle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

void Paddle::handleMouse(float mouseX) {
    float newX = std::clamp(mouseX - shape.getSize().x / 2, 0.f, 730.f - shape.getSize().x);
    shape.setPosition({newX, shape.getPosition().y});
}

void Paddle::changeSize(float newSize) {
    shape.setSize({newSize, shape.getSize().y});
}

sf::FloatRect Paddle::getBounds() const {
    return shape.getGlobalBounds();
}

float Paddle::getWidth() const {
    return shape.getSize().x;
}

sf::Vector2f Paddle::getPosition() const {
    return shape.getPosition();
}
sf::Vector2f Paddle::getSize() const {
    return shape.getSize();
}

void Paddle::setSize(const sf::Vector2f& newSize) {
    shape.setSize(newSize);
}