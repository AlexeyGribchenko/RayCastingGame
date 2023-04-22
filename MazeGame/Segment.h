#pragma once

#include <SFML/Graphics.hpp>

struct Segment
{
	sf::Vector2f start_point;
	sf::Vector2f end_point;

	Segment();

	Segment(sf::Vector2f start, sf::Vector2f end);
};