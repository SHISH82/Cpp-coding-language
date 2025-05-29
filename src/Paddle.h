#pragma once
#include <SFML/Graphics.hpp>

class Paddle {
public:
    Paddle(float x, float y, float initialWidth = 140.f);
    void update();
    void draw(sf::RenderWindow& window) const;

    void handleMouse(float mouseX);
    void changeSize(float newSize);
    void setSize(const sf::Vector2f& newSize);

    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getSize() const;

    float getWidth() const;

    bool hasBottomPass = false;

private:
    sf::RectangleShape shape;
    float speed = 5.f;
};