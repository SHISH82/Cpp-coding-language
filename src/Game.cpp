#include "Game.h"
#include <cstdlib>
#include <ctime>
#include "iostream"
#include <random>

Game::Game()
    : window(sf::VideoMode(sf::Vector2u(730, 730)), "Arkanoid", sf::Style::Default),
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
        bricks.emplace_back(Brick(pos));
    }


    for (int i = 0; i < 3 && i < bricks.size(); ++i) {
        bricks[i] = Brick(bricks[i].shape.getPosition(), true);
    }
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
            }else if constexpr (std::is_same_v<T, sf::Event::MouseMoved>) {

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


    auto ballBounds = ball.shape.getGlobalBounds();
    auto paddleBounds = paddle.shape.getGlobalBounds();

    if (ballBounds.findIntersection(paddleBounds)) {
        ball.velocity.y = -std::abs(ball.velocity.y);
    }

    bool collisionDetected = false;


    for (auto& brick : bricks) {
        auto brickBounds = brick.shape.getGlobalBounds();

        if (!collisionDetected && ball.shape.getGlobalBounds().findIntersection(brickBounds)) {

            if (!brick.isIndestructible) {
                brick.health--;


                if (brick.health == 2) {
                    brick.shape.setFillColor(sf::Color::Yellow);
                } else if (brick.health == 1) {
                    brick.shape.setFillColor(sf::Color::Red);
                } else {
                    brick.shape.setFillColor(sf::Color::Black);
                }


                if (brick.hasBonus && brick.health == 2) {
                    bonuses.emplace_back(Bonus(brick.shape.getPosition(), brick.bonusType));
                    brick.hasBonus = false;
                }
            }


            if (ball.velocity.y > 0) {

                ball.shape.setPosition({
                    ball.shape.getPosition().x,
                    brick.shape.getPosition().y - ball.shape.getRadius() * 2
                });
            } else {

                ball.shape.setPosition({
                    ball.shape.getPosition().x,
                    brick.shape.getPosition().y + brick.shape.getSize().y
                });
            }


            ball.velocity.y *= -1;


            ball.accelerate(1.01f);

            collisionDetected = true;
        }
    }


    bricks.erase(
        std::remove_if(bricks.begin(), bricks.end(), [](const Brick& b) {
            return !b.isIndestructible && b.health <= 0;
        }),
        bricks.end()
    );


    for (auto it = bonuses.begin(); it != bonuses.end();) {
        Bonus& bonus = *it;
        bonus.update();

        if (bonus.shape.getGlobalBounds().findIntersection(paddle.shape.getGlobalBounds())) {
            if (!bonus.isCollected()) {
                switch (bonus.type) {
                    case BonusType::EnlargePaddle:
                        paddle.changeSize(paddle.shape.getSize().x * 1.1f);
                        break;
                    case BonusType::SpeedUpBall:
                        ball.accelerate(1.1f);
                        break;
                    case BonusType::BottomPass:
                        hasBottomPlatform = true;
                        bottomPlatform.setSize({730.f, 20.f});
                        bottomPlatform.setFillColor(sf::Color::Cyan);
                        bottomPlatform.setPosition({0.f, 730.f - 20.f});
                        break;
                    case BonusType::RandomTrajectory:
                        hasRandomTrajectory = true;
                        randomTrajTimer.restart();
                        break;
                }

                bonus.collect();
            }
        }


        if (bonus.shape.getPosition().y > 730) {
            it = bonuses.erase(it);
        } else {
            ++it;
        }
    }


    bonuses.erase(
        std::remove_if(bonuses.begin(), bonuses.end(), [](const Bonus& b) {
            return b.isCollected();
        }),
        bonuses.end()
    );



    if (hasRandomTrajectory) {
        if (randomTrajTimer.getElapsedTime().asSeconds() < 2.0f) {
            if (rand() % 200 == 0) {
                float randomAngle = static_cast<float>(rand() % 360 - 180);
                float angleRad = randomAngle * M_PI / 180.f;
                float currentSpeed = std::sqrt(ball.velocity.x * ball.velocity.x + ball.velocity.y * ball.velocity.y);

                ball.velocity.x = currentSpeed * cos(angleRad);
                ball.velocity.y = currentSpeed * sin(angleRad);
            }
        } else {
            hasRandomTrajectory = false;
        }
    }



    if (hasBottomPlatform) {
        auto platformBounds = bottomPlatform.getGlobalBounds();
        if (ball.shape.getGlobalBounds().findIntersection(platformBounds)) {
            ball.velocity.y *= -1;
            hasBottomPlatform = false;
        }
    }


    float ballBottom = ball.shape.getPosition().y + ball.shape.getRadius() * 2;
    if (ballBottom > 730) {
        if (hasBottomPlatform) {
            ball.velocity.y *= -1;
            hasBottomPlatform = false;
        } else {
            ball.isLaunched = false;

            sf::Vector2f size = paddle.shape.getSize();
            float newWidth = size.x * 0.9f;
            paddle.shape.setSize({newWidth, size.y});

            ball.reset(paddle);
        }
    }
}


void Game::render() {
    window.clear();

    paddle.draw(window);
    ball.draw(window);

    for (const auto& brick : bricks) {
        brick.draw(window);
    }

    for (const auto& bonus : bonuses) {
        bonus.draw(window);
    }
    if (hasBottomPlatform) {
        window.draw(bottomPlatform);
    }


    window.display();
}