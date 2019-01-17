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

using BoidList = std::vector<Boid>;

const int WIDTH  = 1280;
const int HEIGHT = 720;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Flocking");
sf::CircleShape shape(10.0f);
BoidList boids;

BoidList getNeighbors(const Boid& boid);
void renderBoids();
void initBoids();

int main() {
  shape.setOrigin(sf::Vector2f(10.0f, 10.0f));
  srand(time(NULL));
  initBoids();

  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    for(Boid& boid: boids) {
      BoidList neighbors = getNeighbors(boid);
      boid.align(neighbors);
      boid.separate(neighbors);
      boid.cohere(neighbors);
      boid.move();
      boid.wrap(WIDTH, HEIGHT);
    }

    window.clear();
    renderBoids();
    window.display();
  }

  return 0;
}

void initBoids()
{
  for(int i = 0; i < BOID_COUNT; i++) {
    Boid boid(WIDTH, HEIGHT);
    boids.push_back(boid);
  }
}

void renderBoids()
{
  for(Boid& boid: boids) {
    shape.setPosition(boid.pos());
    window.draw(shape);
  }
}

BoidList getNeighbors(const Boid& boid) {
  BoidList list;
  for (const Boid& other: boids) {
    if (&boid == &other) continue;
    if (magnitude(boid.pos() - other.pos()) <= PERC_RAD)
      list.push_back(other);
  }
  return list;
}

