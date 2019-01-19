#pragma once

#include "engine/gamelogic.hpp"
#include "./boid.h"
#include <SFML/Graphics.hpp>

class Flocking: public Engine::IGameLogic
{
public:
  void init() override;
  void update() override;
  void render() override;
private:
  std::vector<Boid> boids;
  std::vector<Boid> getNeighbors(const Boid& boid);
  sf::CircleShape shape = sf::CircleShape(10.0f);
};

