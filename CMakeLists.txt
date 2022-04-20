cmake_minimum_required(VERSION 3.0.0)
project(algo3-tp1 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(ejercicio1 ./ejercicios/1/main.cpp ./common/clique.cpp ./common/clique.h)
add_executable(ejercicio2 ./ejercicios/2/main.cpp ./common/clique.cpp ./common/clique.h)
add_executable(ejercicio3 ./ejercicios/3/main.cpp ./common/clique.cpp ./common/clique.h)
add_executable(ejercicio4 ./ejercicios/4/main.cpp common/actividad.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
