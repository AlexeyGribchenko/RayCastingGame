#pragma once

#include "Player.h"
#include "Map.h"

class RayCasting
{
public:

	static void drawScene(sf::RenderWindow& window, Player& player, Map& world_map, sf::Texture& texture);

	static void screen2(sf::RenderWindow& window, Player& player, Map& world_map);

	static void screen3(sf::RenderWindow& window, Player& player, Map& world_map);

private:

	static float getDistance(Segment ray, Segment segment);

	static void drawFloorAndSky(sf::RenderWindow& window, float horizont_pos);

	static void drawWall(sf::RenderWindow& window, float i_angle, float distance, float offset, sf::Texture& texture, float v_pos);
};