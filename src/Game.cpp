#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <random>

Game::Game()
: window(sf::VideoMode({730, 730}), "Arkanoid", sf::Style::Default),
      paddle(300, 700, 140.f),
      ball(350, 350)
{
    srand(static_cast<unsigned>(time(nullptr)));
    initBricks();
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::initBricks() {
    constexpr int cols = 9;
    constexpr int rows = 9;
    constexpr float brickWidth = 80.f;
    constexpr float brickHeight = 20.f;

    float totalWidth = cols * brickWidth;
    float offsetX = (window.getSize().x - totalWidth) / 2.f;

    std::vector<sf::Vector2f> positions;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            float x = offsetX + col * brickWidth;
            float y = 50.f + row * brickHeight;
            positions.push_back({x, y});
        }
    }

    std::shuffle(positions.begin(), positions.end(), std::mt19937{std::random_device{}()});

    for (const auto& pos : positions) {
        bricks.emplace_back(std::make_unique<Brick>(pos));
    }

    for (int i = 0; i < 3 && i < bricks.size(); ++i) {
        sf::FloatRect bounds = bricks[i]->getBounds();
        bricks[i] = std::make_unique<Brick>(sf::Vector2f(bounds.position.x, bounds.position.y), true);
    }
    }
void Game::activateBottomPlatform() {
    hasBottomPlatform = true;
    bottomPlatform.setSize({730.f, 20.f});
    bottomPlatform.setFillColor(sf::Color::Cyan);
    bottomPlatform.setPosition({0.f, 730.f - 20.f});
}
void Game::activateRandomTrajectory() {
    hasRandomTrajectory = true;
    randomTrajTimer.restart();
}


void Game::processEvents() {
    while (auto event = window.pollEvent()) {
        event->visit([&](const auto& e) {
            using T = std::decay_t<decltype(e)>;

            if constexpr (std::is_same_v<T, sf::Event::Closed>) {
                window.close();
            } else if constexpr (std::is_same_v<T, sf::Event::KeyPressed>) {
                if (e.code == sf::Keyboard::Key::Escape) {
                    window.close();
                } else if (e.code == sf::Keyboard::Key::Space) {
                    ball.isLaunched = true;
                }
            } else if constexpr (std::is_same_v<T, sf::Event::MouseButtonPressed>) {
                if (!ball.isLaunched) {
                    ball.setDirectionFromMouse(window);
                }
            }
        });
    }
}

void Game::update() {
    float mouseX = static_cast<float>(sf::Mouse::getPosition(window).x);
    if (ball.isLaunched) {
        paddle.handleMouse(mouseX);
    }

    ball.update(paddle);
    paddle.update();

    // Проверка столкновения мяча с платфорой
    auto ballBounds = ball.getShape().getGlobalBounds();
    auto paddleBounds = paddle.getBounds();

    if (ballBounds.findIntersection(paddleBounds)) {
        ball.setVelocity({ball.getVelocity().x, -std::abs(ball.getVelocity().y)});
    }

    bool collisionDetected = false;

    // Проверка столкновений мяча с кирпичами
    for (auto& brick : bricks) {
        auto brickBounds = brick->getBounds();

        if (!collisionDetected && ball.getShape().getGlobalBounds().findIntersection(brickBounds)) {
            if (!brick->isIndestructible) {
                brick->hit();

                // Выдать бонус
                if (brick->hasBonus && brick->getHealth() == 2) {
                    if (auto bonus = brick->releaseBonus()) {
                        bonuses.push_back(std::move(bonus));
                    }
                }
            }

            // Обновление позиции
            if (ball.getVelocity().y > 0) {
                ball.getShape().setPosition({
                    ball.getShape().getPosition().x,
                    brick->getBounds().position.y - ball.getShape().getRadius() * 2
                });
            } else {
                ball.getShape().setPosition({
                    ball.getShape().getPosition().x,
                    brick->getBounds().position.y + brick->getBounds().size.y
                });
            }

            ball.setVelocity({ball.getVelocity().x, -ball.getVelocity().y});
            ball.accelerate(1.01f);

            collisionDetected = true;
        }
    }

    // Удаление разрушенных кирпичей
    bricks.erase(
        std::remove_if(bricks.begin(), bricks.end(),
                       [](const std::unique_ptr<Brick>& b) { return b->isDestroyed(); }),
        bricks.end());

    // Обработка бонусов
    for (auto it = bonuses.begin(); it != bonuses.end();) {
        Bonus* bonus = it->get();
        bonus->update();

        if (bonus->getShape().getGlobalBounds().findIntersection(paddle.getBounds())) {
            if (!bonus->isCollected()) {
                bonus->applyEffect(paddle, ball, *this);  // Полиморфический вызов
                bonus->collect();
            }
        }

        if (bonus->getShape().getPosition().y > 730) {
            it = bonuses.erase(it);
        } else {
            ++it;
        }
    }

    // Удаление собранных бонусов
    bonuses.erase(
        std::remove_if(bonuses.begin(), bonuses.end(),
                       [](const std::unique_ptr<Bonus>& b) { return b->isCollected(); }),
        bonuses.end());



    // Проверка выхода мяча за нижнюю границу
    float ballBottom = ball.getShape().getPosition().y + ball.getShape().getRadius() * 2;
    if (ballBottom > 730) {
        if (hasBottomPlatform) {
            ball.setVelocity({ball.getVelocity().x, -ball.getVelocity().y});
            hasBottomPlatform = false;
        } else {
            ball.isLaunched = false;
            sf::Vector2f size = paddle.getSize();
            float newWidth = size.x * 0.9f;
            paddle.setSize({newWidth, size.y});
            ball.reset(paddle);
        }
    }
}

void Game::render() {
    window.clear();

    paddle.draw(window);
    ball.draw(window);

    for (const auto& brick : bricks) {
        brick->draw(window);
    }

    for (const auto& bonus : bonuses) {
        bonus->draw(window);
    }

    if (hasBottomPlatform) {
        window.draw(bottomPlatform);
    }

    window.display();
}