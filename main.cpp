#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <assert.h>
#include "./boid.h"
#include "./utils.h"


using BoidList = std::vector<Boid>;

const float ALIGNMENT_FACTOR  = 1;
const float SEPARATION_FACTOR = 1;
const float COHESION_FACTOR   = 0.5;
const int PERC_RAD            = 100;
const float MAX_VEL           = 1;
const float MAX_FORCE         = 0.001f;
const int BOID_COUNT          = 200;

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
    if (neighbors.size() == 0) continue;
    sf::Vector2f sum;
    for (const Boid& other: neighbors) {
      sum += other.vel;
    }
    sf::Vector2f avg(sum.x / (float)neighbors.size(), sum.y / (float)neighbors.size());
    sf::Vector2f steer = avg - boid.vel;
    steer *= ALIGNMENT_FACTOR;
    boid.acc += steer;
  }
}

void separate()
{
  for(Boid& boid: boids) {
    BoidList neighbors = getNeighbors(boid);
    if (neighbors.size() == 0) continue;
    sf::Vector2f sum;
    for (const Boid& other: neighbors) {
      sf::Vector2f diff = boid.pos - other.pos;
      float factor = pow(magnitude(diff), 2);
      sum += sf::Vector2f(diff.x / factor, diff.y / factor);
    }
    sf::Vector2f avg(sum.x / (float)neighbors.size(), sum.y / (float)neighbors.size());
    sf::Vector2f steer = avg - boid.vel;
    steer *= SEPARATION_FACTOR;
    boid.acc += steer;
  }
}

void cohesion()
{
  for(Boid& boid: boids) {
    BoidList neighbors = getNeighbors(boid);
    if (neighbors.size() == 0) continue;
    sf::Vector2f sum;
    for (const Boid& other: neighbors) {
      sum += other.pos;
    }
    sf::Vector2f avg(sum.x / (float)neighbors.size(), sum.y / (float)neighbors.size());
    sf::Vector2f steer = avg - boid.pos;
    steer -= boid.vel;
    steer *= COHESION_FACTOR;
    boid.acc += steer;
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

