#include "Paddle.h"
#include "iostream"

Paddle::Paddle(float x, float y, float initialWidth){
    shape.setSize({initialWidth, 20});
    shape.setFillColor(sf::Color::White);
    shape.setPosition(sf::Vector2f(x, y));
}

void Paddle::update() {}

void Paddle::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Paddle::handleMouse(float mouseX) {
    float newX = std::clamp(mouseX - shape.getSize().x / 2, 0.f, 730.f - shape.getSize().x);
    shape.setPosition({newX, shape.getPosition().y});
}
void Paddle::changeSize(float newSize) {
    shape.setSize({newSize, shape.getSize().y});
}
