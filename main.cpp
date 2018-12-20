#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <assert.h>
#include "./boid.h"


using BoidList = std::vector<Boid>;

const int WIDTH  = 1280;
const int HEIGHT = 720;
const int BOID_COUNT = 100;
const int PERC_RAD = 50;
const float MAX_VEL = 0.2f;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Flocking");
sf::CircleShape shape(10.0f);
sf::CircleShape neighbor(10.0f);
sf::CircleShape perception(PERC_RAD);
BoidList boids;

BoidList getNeighbors(const Boid& boid);
void align();
void renderBoids();
void initBoids();
void wrap();
void setMagnitude(sf::Vector2f&, float);
float magnitude(const sf::Vector2f&);

int main() {
  shape.setOrigin(sf::Vector2f(10.0f, 10.0f));
  neighbor.setOrigin(sf::Vector2f(10.0f, 10.0f));
  neighbor.setFillColor(sf::Color::Red);
  perception.setOrigin(sf::Vector2f(PERC_RAD, PERC_RAD));
  perception.setFillColor(sf::Color::Yellow);
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

    for(Boid& boid: boids) {
      boid.vel += boid.acc;
      if (magnitude(boid.vel) > MAX_VEL)
        setMagnitude(boid.vel, MAX_VEL);
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
    boid.acc.x *= -1;
    boid.acc.y *= -1;
    boid.pos.x = rand() % WIDTH;
    boid.pos.y = rand() % HEIGHT;
    boids.push_back(boid);
  }
}

void renderBoids()
{
  perception.setPosition(boids[0].pos);
  window.draw(perception);
  for(Boid& boid: boids) {
    shape.setPosition(boid.pos);
    window.draw(shape);
  }
  for(Boid boid: getNeighbors(boids[0])) {
    neighbor.setPosition(boid.pos);
    window.draw(neighbor);
  }
}

sf::Vector2f normalize(const sf::Vector2f& vec)
{
  sf::Vector2f normalized;
  float mag = magnitude(vec);
  normalized.x = vec.x / mag;
  normalized.y = vec.y / mag;
  return normalized;
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

void setMagnitude(sf::Vector2f& vec, float mag)
{
  float currentMag = magnitude(vec);
  float factor = mag / currentMag;
  vec.x *= factor;
  vec.y *= factor;
}

float magnitude(const sf::Vector2f& vec)
{
  return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
}

