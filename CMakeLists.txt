cmake_minimum_required(VERSION 4.0.2)
project(TurgauFighter C)

set(CMAKE_C_STANDARD 11)

# Include raylib's source directory directly
add_subdirectory("C:/raylib/raylib" ${CMAKE_BINARY_DIR}/raylib)

add_compile_options(-O3 -Ofast -march=native -ffast-math)

add_executable(TurgauFighter
        main.c
        src/init.c
        src/include/init.h
        src/GameLoop.c
        src/Windows.c
        src/include/Windows.h
        src/logic.c
        src/include/logic.h
        src/transform.c
        src/include/transform.h
)

# Link against the target 'raylib' target built from the subdirectory
target_link_libraries(${PROJECT_NAME}
        raylib
        opengl32
        gdi32
        winmm
)