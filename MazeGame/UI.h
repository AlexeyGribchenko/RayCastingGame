#pragma once

#include "Map.h"
#include "Player.h"
#include "Settings.h"
#include "SFML/Graphics.hpp"

class UI
{
	sf::RenderWindow* pWindow;
	sf::Text fps_text;
	sf::Font fps_font;
public:

	UI(sf::RenderWindow& window);

	void drawMiniMap(Map& world_map, Player& player);

	void drawCrosshair();

	void drawFPS(sf::Clock& clock);
};