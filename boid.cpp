#include "./boid.h"

#include <vector>
#include "params.h"

using BoidList = std::vector<Boid>;

void Boid::align(const BoidList& neighbors)
{
  if (neighbors.size() == 0) return;
  sf::Vector2f sum;
  for (const Boid& other: neighbors) {
    sum += other.vel;
  }
  sf::Vector2f avg(
      sum.x / (float)neighbors.size(),
      sum.y / (float)neighbors.size());
  sf::Vector2f steer = avg - this->vel;
  steer *= ALIGNMENT_FACTOR;
  this->acc += steer;
}

void Boid::separate(const BoidList& neighbors)
{
  if (neighbors.size() == 0) return;
  sf::Vector2f sum;
  for (const Boid& other: neighbors) {
    sf::Vector2f diff = this->pos - other.pos;
    float factor = pow(magnitude(diff), 2);
    sum += sf::Vector2f(diff.x / factor, diff.y / factor);
  }
  sf::Vector2f avg(sum.x / (float)neighbors.size(), sum.y / (float)neighbors.size());
  sf::Vector2f steer = avg - this->vel;
  steer *= SEPARATION_FACTOR;
  this->acc += steer;
}

void Boid::cohere(const BoidList& neighbors)
{
  if (neighbors.size() == 0) return;
  sf::Vector2f sum;
  for (const Boid& other: neighbors) {
    sum += other.pos;
  }
  sf::Vector2f avg(sum.x / (float)neighbors.size(), sum.y / (float)neighbors.size());
  sf::Vector2f steer = avg - this->pos;
  steer -= this->vel;
  steer *= COHESION_FACTOR;
  this->acc += steer;
}

