#include "./boid.h"

#include <vector>
#include "params.h"

using BoidList = std::vector<Boid>;

Boid::Boid(int width, int height) {
    _acc.x = rand() / (float)RAND_MAX - 0.5;
    _acc.y = rand() / (float)RAND_MAX - 0.5;
    _pos.x = rand() % width;
    _pos.y = rand() % height;
}

sf::Vector2f Boid::pos() const
{
  return this->_pos;
}

sf::Vector2f Boid::vel() const
{
  return this->_vel;
}

sf::Vector2f Boid::acc() const
{
  return this->_acc;
}

void Boid::align(const BoidList& neighbors)
{
  if (neighbors.size() == 0) return;
  sf::Vector2f sum;
  for (const Boid& other: neighbors) {
    sum += other.vel();
  }
  sf::Vector2f avg(
      sum.x / (float)neighbors.size(),
      sum.y / (float)neighbors.size());
  sf::Vector2f steer = avg - _vel;
  steer *= ALIGNMENT_FACTOR;
  _acc += steer;
}

void Boid::separate(const BoidList& neighbors)
{
  if (neighbors.size() == 0) return;
  sf::Vector2f sum;
  for (const Boid& other: neighbors) {
    sf::Vector2f diff = _pos - other.pos();
    float factor = pow(magnitude(diff), 2);
    sum += sf::Vector2f(diff.x / factor, diff.y / factor);
  }
  sf::Vector2f avg(sum.x / (float)neighbors.size(), sum.y / (float)neighbors.size());
  sf::Vector2f steer = avg - _vel;
  steer *= SEPARATION_FACTOR;
  _acc += steer;
}

void Boid::cohere(const BoidList& neighbors)
{
  if (neighbors.size() == 0) return;
  sf::Vector2f sum;
  for (const Boid& other: neighbors) {
    sum += other.pos();
  }
  sf::Vector2f avg(sum.x / (float)neighbors.size(), sum.y / (float)neighbors.size());
  sf::Vector2f steer = avg - _pos;
  steer -= _vel;
  steer *= COHESION_FACTOR;
  _acc += steer;
}

void Boid::move()
{
  limit(_acc, MAX_FORCE);
  _vel += _acc;
  limit(_vel, MAX_VEL);
  _pos += _vel;
}

void Boid::wrap(int width, int height)
{
  if (_pos.x >= width) _pos.x -= width;
  else if (_pos.x < 0) _pos.x += width;

  if (_pos.y >= height) _pos.y -= height;
  else if (_pos.y < 0) _pos.y += height;
}

