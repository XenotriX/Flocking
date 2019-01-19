#include "engine/game.hpp"
#include "./flocking.hpp"

int main()
{
  Engine::Game game("Flocking", 1280, 720);
  Flocking flocking;
  game.Start(&flocking);
  return 0;
}

