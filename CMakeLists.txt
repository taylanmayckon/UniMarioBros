cmake_minimum_required(VERSION 3.12)
project(UniMarioBros C)

# Mudando o local em que o arquivo exe é gerado
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_C_STANDARD 99)

# Para chamar a pasta lib de forma mais elegante no codigo fonte
include_directories(${CMAKE_SOURCE_DIR}/lib) 

# Para não compilar os exemplos da raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# Adicionando o diretorio da raylib
add_subdirectory(external/raylib-5.5)
# Pasta dos headers da raylib
include_directories(external/raylib-5.5/src)

add_executable(UniMarioBros main.c lib/mario.c lib/mario_animdb.c)
target_link_libraries(UniMarioBros raylib)
