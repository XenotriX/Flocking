#pragma once

#include <vector>
#include <SFML/System.hpp>
#include "./utils.h"
#include <cmath>

class Boid
{
public:
  sf::Vector2f pos;
  sf::Vector2f vel;
  sf::Vector2f acc;
  void align(const std::vector<Boid>&);
  void separate(const std::vector<Boid>&);
  void cohere(const std::vector<Boid>&);
};

