#include "quadtree.hpp"
#include "params.h"

#include <iostream>

QuadTree::QuadTree(const sf::FloatRect& boundaries, unsigned char depth/*= 0*/) :
  depth(depth), boundaries(boundaries)
{

}

void QuadTree::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  sf::RectangleShape rectangle(sf::Vector2f(boundaries.width, boundaries.height));
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setOutlineThickness(1);
  rectangle.setOutlineColor(sf::Color(36, 36, 36));
  rectangle.setPosition(sf::Vector2f(boundaries.left, boundaries.top));
  target.draw(rectangle);
  if (subdivided) {
    target.draw(*topleft);
    target.draw(*topright);
    target.draw(*botleft);
    target.draw(*botright);
  }
}

void QuadTree::Subdivide()
{
  if (subdivided)
    return;
  //std::cout << "Subdividing" << std::endl;
  topleft  = new QuadTree(sf::FloatRect(
        boundaries.left,
        boundaries.top,
        boundaries.width / 2,
        boundaries.height / 2), depth+1);
  topright = new QuadTree(sf::FloatRect(
        boundaries.left + boundaries.width / 2,
        boundaries.top,
        boundaries.width / 2,
        boundaries.height / 2), depth+1);
  botleft  = new QuadTree(sf::FloatRect(
        boundaries.left,
        boundaries.top + boundaries.height / 2,
        boundaries.width / 2,
        boundaries.height / 2), depth+1);
  botright = new QuadTree(sf::FloatRect(
        boundaries.left + boundaries.width / 2,
        boundaries.top + boundaries.height / 2,
        boundaries.width / 2,
        boundaries.height / 2), depth+1);
  subdivided = true;

  for (const Boid& boid : boids) {
    Insert(boids.back());
    boids.pop_back();
  }
}

void QuadTree::Insert(const Boid& boid)
{
  if (!subdivided &&
      (boids.size() >= BOID_PER_QUAD) &&
      depth <= MAX_DEPTH)
    Subdivide();
  if (subdivided) {
    // Right
    if (boid.pos().x + PERC_RAD > boundaries.left + boundaries.width / 2) {
      // Bottom
      if (boid.pos().y + PERC_RAD > boundaries.top + boundaries.height / 2)
        botright->Insert(boid);
      // Right
      if (boid.pos().y - PERC_RAD < boundaries.top + boundaries.height / 2)
        topright->Insert(boid);
    }
    // Left
    if (boid.pos().x - PERC_RAD < boundaries.left + boundaries.width / 2) {
      // Bottom
      if (boid.pos().y + PERC_RAD > boundaries.top + boundaries.height / 2)
        botleft->Insert(boid);
      // Top
      if (boid.pos().y - PERC_RAD < boundaries.top + boundaries.height / 2)
        topleft->Insert(boid);
    }
  }
  else {
    //std::cout << "Inserting" << std::endl;
    boids.push_back(boid);
  }
}

std::vector<Boid> QuadTree::GetNeighbors(const Boid& boid)
{
  if (!boundaries.contains(boid.pos()))
    return std::vector<Boid>();
  if (subdivided) {
    if (boid.pos().x > boundaries.left + boundaries.width / 2) {
      if (boid.pos().y > boundaries.top + boundaries.height / 2)
        return botright->GetNeighbors(boid);
      else
        return topright->GetNeighbors(boid);
    } else {
      if (boid.pos().y > boundaries.top + boundaries.height / 2)
        return botleft->GetNeighbors(boid);
      else
        return topleft->GetNeighbors(boid);
    }
  } else {
    std::vector<Boid> neighbors;
    for (auto neighbor: boids) {
      if (boid.pos() != neighbor.pos())
        neighbors.push_back(neighbor);
    }
    return neighbors;
  }
}

