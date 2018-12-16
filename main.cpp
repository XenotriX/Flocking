#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include "./boid.h"


using BoidList = std::vector<Boid>;

const int WIDTH  = 1280;
const int HEIGHT = 720;
const int BOID_COUNT = 100;
const int PERC_RAD = 100;
const float MAX_VEL = 0.2f;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Flocking");
sf::CircleShape shape(10.0f);
BoidList boids;

void renderBoids();
void initBoids();
void wrap();
void setMagnitude(sf::Vector2f&, float);
float magnitude(const sf::Vector2f&);

int main() {
  srand(time(NULL));
  initBoids();

  while(window.isOpen())
  {
    sf::Event event;
    while(window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

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
    boid.acc.x = (rand() % 4 - 2) / (float)100;
    boid.acc.y = (rand() % 4 - 2) / (float)100;
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

