#pragma once

#include <vector>
#include <SFML/System.hpp>
#include "./utils.h"
#include <cmath>

class Boid
{
private:
  sf::Vector2f _pos;
  sf::Vector2f _vel;
  sf::Vector2f _acc;
public:
  Boid(int, int);
  sf::Vector2f pos() const;
  sf::Vector2f vel() const;
  sf::Vector2f acc() const;
  void align(const std::vector<Boid>&);
  void separate(const std::vector<Boid>&);
  void cohere(const std::vector<Boid>&);
  void move();
  void wrap(int, int);
};

