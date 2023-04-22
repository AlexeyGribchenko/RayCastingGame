#include <SFML/Graphics.hpp>
#include "Maze.h"
#include "Map.h"
#include "Player.h"
#include "RayCasting.h"
#include "Settings.h"
#include "App.h"
#include "UI.h"
#include <iostream>

using namespace sf;

void App::Run()
{
	RenderWindow window(VideoMode(WIDTH / 2, HEIGHT / 2), "RayCasting Programme", Style::Fullscreen);
	window.setMouseCursorVisible(false);

	Mouse::setPosition(Vector2i(WIDTH / 2, HEIGHT / 2), window);

	Event event;

	Map world_map(FIRST_LEVEL_SIZE, FIRST_LEVEL_SIZE, 0);
	UI ui(window);

	Player player;

	Texture texture;
	texture.loadFromFile("brick_wall.png");

	Clock fps_clock;
	fps_clock.restart();
	float lastTime = 0;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
			{
				window.close();
			}
		}

		window.clear();

		player.update(window, world_map);
		RayCasting::drawScene(window, player, world_map, texture);

		ui.drawCrosshair();
		ui.drawMiniMap(world_map, player);
		ui.drawFPS(fps_clock);

		window.display();
	}
}