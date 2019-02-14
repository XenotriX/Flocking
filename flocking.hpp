#pragma once

#include "engine/gamelogic.hpp"
#include "./boid.h"
#include <SFML/Graphics.hpp>
#include "./quadtree.hpp"

class Flocking: public Engine::IGameLogic
{
public:
  void init() override;
  void update() override;
  void render() override;
private:
  QuadTree qTree = QuadTree(sf::FloatRect(0, 0, 0, 0));
  std::vector<Boid> boids;
  std::vector<Boid> getNeighbors(const Boid& boid);
};

