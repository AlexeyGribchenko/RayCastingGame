#pragma once
#include "Map.h"
#include "Settings.h"

class Player
{
private:

	float max_speed;
	sf::Vector2f position;
	float angle;
	float speed;
	float view_vert_pos;

public:

	Player();

	void update(sf::RenderWindow& window, Map& map);

	float getAngle();

	float getViewVertPos();
	
	sf::Vector2f getPosition();

	void setPosition(sf::Vector2f new_position);
};
