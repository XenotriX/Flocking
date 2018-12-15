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

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Flocking");
sf::CircleShape shape(10.0f);
BoidList boids;

void renderBoids();
void initBoids();
void wrap();

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
      boid.pos += boid.vel;
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
    Boid boid;
    boid.vel.x = (rand() % 4 - 2) / (float)10;
    boid.vel.y = (rand() % 4 - 2) / (float)10;
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

