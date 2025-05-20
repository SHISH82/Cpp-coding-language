#include "GameLogic.h"

GameLogic::GameLogic() :
    window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Crystal Match"),
    selected(-1, -1),
    matchFound(false)
{


    srand(static_cast<unsigned int>(time(nullptr)));
    initBoard();
}

void GameLogic::initBoard() {
    board = std::vector<std::vector<Tile>>(BOARD_SIZE, std::vector<Tile>(BOARD_SIZE));
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j].spawn(j, i);
        }
    }

    if (!textures[0].loadFromFile("gem_red.png")) std::cerr << "Failed to load gem_red.png\n";
    if (!textures[1].loadFromFile("gem_green.png")) std::cerr << "Failed to load gem_green.png\n";
    if (!textures[2].loadFromFile("gem_blue.png")) std::cerr << "Failed to load gem_blue.png\n";
    if (!textures[3].loadFromFile("gem_yellow.png")) std::cerr << "Failed to load gem_yellow.png\n";
    if (!textures[4].loadFromFile("gem_purple.png")) std::cerr << "Failed to load gem_purple.png\n";
    if (!textures[5].loadFromFile("gem_orange.png")) std::cerr << "Failed to load gem_orange.png\n";

    if (!bonusTexture.loadFromFile("bonus.png")) std::cerr << "Failed to load bonus.png\n";
    if (!bombTexture.loadFromFile("bomb.png")) std::cerr << "Failed to load bomb.png\n";

    // проверка размеров текстур
    for (int i = 0; i < 6; ++i) {
        auto size = textures[i].getSize();
        std::cout << "Texture[" << i << "] loaded: " << size.x << "x" << size.y << "\n";
    }
}

void GameLogic::run() {
    while (window.isOpen()) {
        handleInput();
        findMatches();
        collapseTiles();
        render();
        window.display();
    }
}

void GameLogic::handleInput() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) window.close();
    }

    //  нажатие левой кнопки
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        sf::Vector2i clicked = sf::Mouse::getPosition(window);
        clicked.x /= TILE_SIZE;
        clicked.y /= TILE_SIZE;

        std::cout << "Mouse click at grid position: " << clicked.x << ", " << clicked.y << "\n";

        if (clicked.x >= 0 && clicked.x < BOARD_SIZE &&
            clicked.y >= 0 && clicked.y < BOARD_SIZE) {

            bool isBonusClick = false;

            //  кликнули ли мышью на бонус/бомбу
            for (auto it = bonuses.begin(); it != bonuses.end(); ++it) {
                if (it->x == clicked.x && it->y == clicked.y && it->active) {
                    if (it->type == 0) processBonus(*it);
                    else processBomb(*it);

                    it->active = false;
                    bonuses.erase(it);
                    isBonusClick = true;
                    break;
                }
            }


            if (!isBonusClick) {
                if (selected.x == -1)
                    selected = clicked;
                else {
                    int oldX = selected.x;
                    int oldY = selected.y;


                    if (oldX >= 0 && oldX < BOARD_SIZE &&
                        clicked.x >= 0 && clicked.x < BOARD_SIZE &&
                        oldY >= 0 && oldY < BOARD_SIZE &&
                        clicked.y >= 0 && clicked.y < BOARD_SIZE) {


                        int dx = abs(oldX - clicked.x);
                        int dy = abs(oldY - clicked.y);

                        if ((dx + dy) == 1 || (dx == 1 && dy == 1)) {
                            std::swap(board[oldY][oldX], board[clicked.y][clicked.x]);

                            findMatches();
                            if (!matchFound) {
                                std::cout << "No match found — undo swap\n";
                                std::swap(board[oldY][oldX], board[clicked.y][clicked.x]);
                            } else {
                                matchFound = false;
                            }
                        } else {
                            std::cout << "Not adjacent — no swap\n";
                        }
                    }

                    selected = { -1, -1 };
                }
            }
        }


         sf::sleep(sf::milliseconds(200));
    }
}




void GameLogic::collapseTiles() {
    for (int col = 0; col < BOARD_SIZE; ++col) {
        int write = BOARD_SIZE - 1;


        for (int row = BOARD_SIZE - 1; row >= 0; --row) {
            if (!matched[row][col]) {
                board[write--][col] = board[row][col];
            }
        }


        for (int row = write; row >= 0; --row) {
            board[row][col].spawn(col, row);
        }
    }
}

void GameLogic::processBonus(const Bonus& bonus) {
    if (!bonus.active)
        return;


    if (bonus.x < 0 || bonus.x >= BOARD_SIZE || bonus.y < 0 || bonus.y >= BOARD_SIZE)
        return;

    int targetColor = board[bonus.y][bonus.x].color;

    std::vector<std::pair<int, int>> nearbyTiles;

    for (int dx = -2; dx <= 2; ++dx) {
        for (int dy = -2; dy <= 2; ++dy) {
            int nx = bonus.x + dx;
            int ny = bonus.y + dy;

            if (nx >= 0 && ny >= 0 && nx < BOARD_SIZE && ny < BOARD_SIZE &&
                board[ny][nx].color != -1) {

                if (nx == bonus.x && ny == bonus.y)
                    continue;

                int distance = abs(nx - bonus.x) + abs(ny - bonus.y);
                if (distance == 1)
                    continue;

                nearbyTiles.emplace_back(nx, ny);
            }
        }
    }

    static std::mt19937 rng(std::random_device{}());
    std::shuffle(nearbyTiles.begin(), nearbyTiles.end(), rng);

    int count = 0;
    for (const auto& [x, y] : nearbyTiles) {
        if (count >= 2)
            break;

        if (x == bonus.x && y == bonus.y)
            continue;


        int count = 0;
        for (const auto& [x, y] : nearbyTiles) {
            if (count >= 2)
                break;


            if (x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE &&
                board[y][x].color != -1) {

                board[y][x].color = targetColor;
                count++;
                }
        }
    }
}

void GameLogic::processBomb(const Bonus& bomb) {
    if (!bomb.active)
        return;

    std::vector<std::pair<int, int>> positions;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j].color != -1) {
                positions.emplace_back(j, i);
            }
        }
    }

    static std::mt19937 rng(std::random_device{}());
    std::shuffle(positions.begin(), positions.end(), rng);

    int destroyedCount = 0;
    for (const auto& [x, y] : positions) {
        if (destroyedCount >= 5)
            break;

        if (x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE &&
            !(x == bomb.x && y == bomb.y)) {
            board[y][x].color = -1;
            destroyedCount++;
        }
    }

    findMatches();
    collapseTiles();
}

void GameLogic::render() {
    std::cout << "Board state:\n";


    window.clear(sf::Color::White);


    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j].color == -1)
                continue;


            if (board[i][j].color >= 0 && board[i][j].color < 6) {
                sf::Sprite tileSprite(textures[board[i][j].color]);
                tileSprite.setPosition(sf::Vector2f(j * TILE_SIZE, i * TILE_SIZE));
                window.draw(tileSprite);
            }

            sf::Sprite tileSprite(textures[board[i][j].color]);
            tileSprite.setPosition(sf::Vector2f(j * TILE_SIZE, i * TILE_SIZE));
            window.draw(tileSprite);
        }
    }


    for (const auto& b : bonuses) {
        if (!b.active)
            continue;

        if (b.x >= 0 && b.y >= 0 && b.x < BOARD_SIZE && b.y < BOARD_SIZE) {
            if (b.type == 0) {
                sf::Sprite bonusSprite(bonusTexture);
                bonusSprite.setPosition(sf::Vector2f(b.x * TILE_SIZE, b.y * TILE_SIZE));
                window.draw(bonusSprite);
            } else {
                sf::Sprite bombSprite(bombTexture);
                bombSprite.setPosition(sf::Vector2f(b.x * TILE_SIZE, b.y * TILE_SIZE));
                window.draw(bombSprite);
            }
        }
    }
}

void GameLogic::findMatches() {
    matched = std::vector<std::vector<bool>>(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, false));
    matchFound = false;


    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j].color == -1) {
                matched[i][j] = true;
            }
        }
    }


    for (int i = 0; i < BOARD_SIZE; ++i) {
        int count = 1;
        for (int j = 1; j < BOARD_SIZE; ++j) {
            if (board[i][j].color == board[i][j - 1].color && board[i][j].color != -1)
                count++;
            else
                count = 1;

            if (count >= 3) {
                matchFound = true;
                for (int k = 0; k < count; ++k) {
                    int currentJ = j - k;
                    if (currentJ >= 0 && currentJ < BOARD_SIZE) {
                        matched[i][currentJ] = true;
                    }
                }
            }
        }
    }


    for (int j = 0; j < BOARD_SIZE; ++j) {
        int count = 1;
        for (int i = 1; i < BOARD_SIZE; ++i) {
            if (board[i][j].color == board[i - 1][j].color && board[i][j].color != -1)
                count++;
            else
                count = 1;

            if (count >= 3) {
                matchFound = true;
                for (int k = 0; k < count; ++k) {
                    int currentI = i - k;
                    if (currentI >= 0 && currentI < BOARD_SIZE) {
                        matched[currentI][j] = true;
                    }
                }
            }
        }
    }


    std::vector<std::pair<int, int>> destroyedTiles;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (matched[i][j]) {
                board[i][j].color = -1;
                destroyedTiles.emplace_back(j, i);
            }
        }
    }

    static std::mt19937 rng(std::random_device{}());

    if (!destroyedTiles.empty()) {
        std::shuffle(destroyedTiles.begin(), destroyedTiles.end(), rng);

        Bonus b;
        b.type = rand() % 2;
        b.x = destroyedTiles[0].first;
        b.y = destroyedTiles[0].second;
        bonuses.push_back(b);
    }
}