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
  std::string input;
  std::getline(std::cin, input);
  int seed;
  if (!input.empty())
    seed = atoi(input.c_str());
  else
    seed = time(NULL);
  std::cout << "Seed: " << seed << "\n";
  srand(seed);
  float top = window->getView().getCenter().y - window->getView().getSize().y/2;
  float left = window->getView().getCenter().x - window->getView().getSize().x/2;
  auto vp = sf::FloatRect(sf::Vector2f(left, top), window->getView().getSize());
  qTree = QuadTree(vp);
  //std::cout << "Creating boids between " << vp.left << "/" << vp.top << " and " << vp.width << "/" << vp.height << std::endl;
  for(int i = 0; i < BOID_COUNT; i++) {
    std::cout << "Creating boid " << i + 1 << "/" << BOID_COUNT;
    Boid boid(vp.width, vp.height);
    boids.push_back(boid);
    qTree.Insert(boid);
    std::cout << "Created boid " << i + 1 << "/" << BOID_COUNT << "/n";
  }
}

void Flocking::update()
{
  //std::cout << "View center: " << window->getView().getCenter().x << "/" << window->getView().getCenter().y << std::endl;
  for(Boid& boid: boids) {
    BoidList neighbors = qTree.GetNeighbors(boid);
    boid.align(neighbors);
    boid.separate(neighbors);
    boid.cohere(neighbors);
    boid.move();
    boid.wrap(window->getSize().x, window->getSize().y);
  }

  float top = window->getView().getCenter().y - window->getView().getSize().y/2;
  float left = window->getView().getCenter().x - window->getView().getSize().x/2;
  auto vp = sf::FloatRect(sf::Vector2f(left, top), window->getView().getSize());
  qTree = QuadTree(vp);

  //std::cout << "Generating new QuadTree" << std::endl;
  for(Boid& boid: boids) {
    qTree.Insert(boid);
  }
}

void Flocking::render()
{
  sf::CircleShape perception = sf::CircleShape(PERC_RAD);
  perception.setOrigin(sf::Vector2f(PERC_RAD, PERC_RAD));
  perception.setFillColor(sf::Color(255, 255, 0, 32));

  sf::CircleShape shape = sf::CircleShape(10.0f);
  shape.setOrigin(sf::Vector2f(10.0f, 10.0f));
  shape.setPointCount(3);
  shape.setFillColor(sf::Color::White);

  window->draw(qTree);
  for(Boid& boid: boids) {
    perception.setPosition(boid.pos());
    window->draw(perception);
  }
  for(Boid& boid: boids) {
    shape.setPosition(boid.pos());
    sf::Vector2f dir = normalize(boid.vel());
    shape.setRotation(atan2(dir.y, dir.x) * 180 / PI + 30 + 180);
    window->draw(shape);
  }

  auto neighbors = qTree.GetNeighbors(boids.at(0));
  for(Boid& boid: neighbors) {
    shape.setFillColor(sf::Color::Red);
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

