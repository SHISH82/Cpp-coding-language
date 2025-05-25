#pragma once
#include <SFML/Graphics.hpp>

class Paddle {
public:
    Paddle(float x, float y, float initialWidth = 140.f);
    void update();
    void draw(sf::RenderWindow& window);
    void handleMouse(float mouseX);
    void changeSize(float newSize);

    bool hasBottomPass = false;
    sf::RectangleShape shape;

private:
    float speed = 5.f;
};