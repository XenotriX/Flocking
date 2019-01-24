#pragma once

#include <SFML/System.hpp>

float magnitude(const sf::Vector2f& vec);

void setMagnitude(sf::Vector2f& vec, float mag);

void limit(sf::Vector2f& vec, float max);

sf::Vector2f normalize(const sf::Vector2f& vec);

