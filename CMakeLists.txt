cmake_minimum_required(VERSION 3.30)
project(crystal_match)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к SFML
set(SFML_ROOT "/opt/homebrew/Cellar/sfml/3.0.1")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIB_DIR "${SFML_ROOT}/lib")

# Добавляем заголовки SFML
include_directories(${SFML_INCLUDE_DIR})

add_executable(crystal_match
        main.cpp
        GameLogic.cpp
        Tile.cpp
)

# Линкуем SFML (для M1)
target_link_libraries(crystal_match PRIVATE
        "${SFML_LIB_DIR}/libsfml-graphics.dylib"
        "${SFML_LIB_DIR}/libsfml-window.dylib"
        "${SFML_LIB_DIR}/libsfml-system.dylib"
)


file(GLOB IMAGE_FILES "${PROJECT_SOURCE_DIR}/assets/*.png")
foreach(image_file ${IMAGE_FILES})
    configure_file(${image_file} ${CMAKE_BINARY_DIR} COPYONLY)
endforeach()