cmake_minimum_required(VERSION 3.30)
project(arcanoid_v1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к SFML (для macOS с Homebrew)
set(SFML_ROOT "/opt/homebrew/Cellar/sfml/3.0.1")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIB_DIR "${SFML_ROOT}/lib")

# Добавляем пути к заголовочным файлам SFML
include_directories(${SFML_INCLUDE_DIR})

# Список исходных файлов
add_executable(arcanoid_v1
        main.cpp
        src/Ball.cpp
        src/Ball.h
        src/Brick.cpp
        src/Brick.h
        src/Bonus.cpp
        src/Bonus.h
        src/Game.cpp
        src/Game.h
        src/Paddle.cpp
        src/Paddle.h
        src/Utils.h
)

# Линкуем SFML
target_link_libraries(arcanoid_v1 PRIVATE
        "${SFML_LIB_DIR}/libsfml-graphics.dylib"
        "${SFML_LIB_DIR}/libsfml-window.dylib"
        "${SFML_LIB_DIR}/libsfml-system.dylib"
)

