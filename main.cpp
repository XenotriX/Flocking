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
void align();
void separate();
void cohesion();
void renderBoids();
void initBoids();
void wrap();

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

    align();
    separate();
    cohesion();

    for(Boid& boid: boids) {
      limit(boid.acc, MAX_FORCE);
      boid.vel += boid.acc;
      limit(boid.vel, MAX_VEL);
      boid.pos += boid.vel;
    }

    wrap();

    window.clear();
    renderBoids();
    window.display();
  }

  return 0;
}

void initBoids()
{
  for(int i = 0; i < BOID_COUNT; i++) {
    Boid boid;
    boid.acc.x = rand() / (float)RAND_MAX - 0.5;
    boid.acc.y = rand() / (float)RAND_MAX - 0.5;
    boid.pos.x = rand() % WIDTH;
    boid.pos.y = rand() % HEIGHT;
    boids.push_back(boid);
  }
}

void renderBoids()
{
  for(Boid& boid: boids) {
    shape.setPosition(boid.pos);
    window.draw(shape);
  }
}

void align()
{
  for(Boid& boid: boids) {
    BoidList neighbors = getNeighbors(boid);
    boid.align(neighbors);
  }
}

void separate()
{
  for(Boid& boid: boids) {
    BoidList neighbors = getNeighbors(boid);
    boid.separate(neighbors);
  }
}

void cohesion()
{
  for(Boid& boid: boids) {
    BoidList neighbors = getNeighbors(boid);
    boid.cohere(neighbors);
  }
}

BoidList getNeighbors(const Boid& boid) {
  BoidList list;
  for (const Boid& other: boids) {
    if (&boid == &other) continue;
    if (magnitude(boid.pos - other.pos) <= PERC_RAD)
      list.push_back(other);
  }
  return list;
}

void wrap()
{
  for(Boid& boid: boids) {
    if (boid.pos.x >= WIDTH) boid.pos.x -= WIDTH;
    else if (boid.pos.x < 0) boid.pos.x += WIDTH;

    if (boid.pos.y >= HEIGHT) boid.pos.y -= HEIGHT;
    else if (boid.pos.y < 0) boid.pos.y += HEIGHT;
  }
}

