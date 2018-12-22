cmake_minimum_required (VERSION 3.13)
project (Flocking)
add_executable(Flocking
  main.cpp
  utils.h)
target_link_libraries(Flocking -lsfml-graphics)
target_link_libraries(Flocking -lsfml-system)
target_link_libraries(Flocking -lsfml-window)
