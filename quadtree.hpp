#pragma once

#include "./boid.h"
#include <SFML/Graphics.hpp>

class QuadTree : public sf::Drawable
{
public:
  QuadTree(const sf::FloatRect& boundaries, unsigned char depth = 0);
  void Subdivide();
  void Insert(const Boid& boid);
  std::vector<Boid> GetNeighbors(const Boid& boid);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
  QuadTree* topleft;
  QuadTree* topright;
  QuadTree* botleft;
  QuadTree* botright;
  bool      subdivided = false;
  unsigned char depth;
  sf::FloatRect  boundaries;
  std::vector<Boid> boids;
};

