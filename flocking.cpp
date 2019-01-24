#include "./flocking.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <assert.h>
#include "./boid.h"
#include "./utils.h"
#include "./params.h"

#define PI 3.14159265

using BoidList = std::vector<Boid>;

void Flocking::init()
{
  shape.setOrigin(sf::Vector2f(10.0f, 10.0f));
  shape.setPointCount(3);
  srand(time(NULL));
  for(int i = 0; i < BOID_COUNT; i++) {
    Boid boid(window->getSize().x, window->getSize().y);
    boids.push_back(boid);
  }
}

void Flocking::update()
{
  for(Boid& boid: boids) {
    BoidList neighbors = getNeighbors(boid);
    boid.align(neighbors);
    boid.separate(neighbors);
    boid.cohere(neighbors);
    boid.move();
    boid.wrap(window->getSize().x, window->getSize().y);
  }
}

void Flocking::render()
{
  for(Boid& boid: boids) {
    shape.setPosition(boid.pos());
    sf::Vector2f dir = normalize(boid.vel());
    shape.setRotation(atan2(dir.y, dir.x) * 180 / PI + 30 + 180);
    window->draw(shape);
  }
}

BoidList Flocking::getNeighbors(const Boid& boid) {
  BoidList list;
  for (const Boid& other: boids) {
    if (&boid == &other) continue;
    if (magnitude(boid.pos() - other.pos()) <= PERC_RAD)
      list.push_back(other);
  }
  return list;
}

