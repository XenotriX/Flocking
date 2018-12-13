#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <vector>

using BoidList = std::vector<sf::Vector2f>;

const int WIDTH  = 1280;
const int HEIGHT = 720;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Flocking");
sf::CircleShape shape(10.0f);
BoidList boids;

void renderBoids();
void initBoids();

int main() {
  std::cout << "Hello, world!" << std::endl;

  initBoids();

  while(window.isOpen())
  {
    sf::Event event;
    while(window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    renderBoids();
    window.display();
  }

  return 0;
}

void initBoids()
{
  for(int i = 0; i < 10; i++) {
    sf::Vector2f pos;
    pos.x = rand() % WIDTH;
    pos.y = rand() % HEIGHT;
    boids.push_back(pos);
  }
}

void renderBoids()
{
  for(sf::Vector2f pos: boids) {
    shape.setPosition(pos);
    window.draw(shape);
  }
}

