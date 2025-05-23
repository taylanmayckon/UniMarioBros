cmake_minimum_required(VERSION 3.12)
project(UniMarioBros C)

set(CMAKE_C_STANDARD 99)


# Para não compilar os exemplos da raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# Adicionando o diretorio da raylib
add_subdirectory(external/raylib-5.5)
# Pasta dos headers da raylib
include_directories(external/raylib-5.5/src)


add_executable(UniMarioBros main.c)
target_link_libraries(UniMarioBros raylib)

