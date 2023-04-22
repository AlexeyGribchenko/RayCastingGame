#include "UI.h"
using namespace sf;

UI::UI(sf::RenderWindow& window)
{
	this->pWindow = &window;

	this->fps_font.loadFromFile("Roboto-Medium.ttf");
	this->fps_text.setFont(this->fps_font);
	this->fps_text.setPosition(20, 15);
	this->fps_text.setFillColor(Color(230, 230, 230));
	this->fps_text.setCharacterSize(30);
}

void UI::drawMiniMap(Map& world_map, Player& player)
{
	Vector2i map_size = world_map.getMapSize();
	RectangleShape square(Vector2f(MAP_SQUARE_TILE, MAP_SQUARE_TILE));
	CircleShape player_body(PLAYER_RADIUS);
	square.setFillColor(Color(150, 150, 200));

	for (int i = 0; i < map_size.y; i++)
	{
		for (int j = 0; j < map_size.x; j++)
		{
			if (world_map.getMapLink()[i][j])
			{
				if (Vector2i(j, i) == world_map.getExitPoint())
				{
					square.setFillColor(Color(255, 0, 0));
					square.setPosition(Vector2f(j, i) * MAP_SQUARE_TILE + Vector2f(WIDTH - map_size.x * MAP_SQUARE_TILE, 0));
					pWindow->draw(square);
					square.setFillColor(Color(150, 150, 200));
					continue;
				}
				square.setPosition(Vector2f(j, i) * MAP_SQUARE_TILE + Vector2f(WIDTH - map_size.x * MAP_SQUARE_TILE, 0));
				pWindow->draw(square);
			}
		}
	}
	VertexArray line(Lines, 2);
	for (int i = 0; i < (*world_map.getSegmentArrayLink()).size(); i++)
	{
		line[0].position = Vector2f((*world_map.getSegmentArrayLink())[i].start_point) / (SQUARE_TILE / MAP_SQUARE_TILE) + Vector2f(WIDTH - map_size.x * MAP_SQUARE_TILE, 0);
		line[1].position = Vector2f((*world_map.getSegmentArrayLink())[i].end_point) / (SQUARE_TILE / MAP_SQUARE_TILE) + Vector2f(WIDTH - map_size.x * MAP_SQUARE_TILE, 0);
		pWindow->draw(line);
	}

	player_body.setFillColor(Color::Green);
	player_body.setPosition(player.getPosition() / (SQUARE_TILE / MAP_SQUARE_TILE) + Vector2f(WIDTH - map_size.x * MAP_SQUARE_TILE - PLAYER_RADIUS, -PLAYER_RADIUS));
	pWindow->draw(player_body);
}

void UI::drawCrosshair()
{
	RectangleShape crosshair(Vector2f(5, 15));
	crosshair.setFillColor(Color::White);

	crosshair.setPosition(Vector2f(WIDTH / 2 - 2.5, HEIGHT / 2 - 20));
	pWindow->draw(crosshair);

	crosshair.setPosition(Vector2f(WIDTH / 2 - 2.5, HEIGHT / 2 + 5));
	pWindow->draw(crosshair);

	crosshair.rotate(90);

	crosshair.setPosition(Vector2f(WIDTH / 2 + 20, HEIGHT / 2 - 2.5));
	pWindow->draw(crosshair);

	crosshair.setPosition(Vector2f(WIDTH / 2 - 5, HEIGHT / 2 - 2.5));
	pWindow->draw(crosshair);
}

void UI::drawFPS(Clock& clock)
{
	float curTime = clock.restart().asMilliseconds();
	float fps = 1000.f / abs(curTime);

	fps_text.setString(std::to_string((int)fps));

	pWindow->draw(fps_text);
}