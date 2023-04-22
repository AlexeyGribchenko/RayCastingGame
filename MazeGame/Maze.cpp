#include <SFML/Graphics.hpp>
#include "Maze.h"
#include "Settings.h"
#include "Mathematics.h"

using namespace sf;

Maze::Maze(int x_size, int y_size)
{
	this->x_size = x_size;
	this->y_size = y_size;

	game_field = new bool* [y_size];
	for (int i = 0; i < y_size; i++)
	{
		game_field[i] = new bool[x_size];
	}

	createLabirinthStart(x_size, y_size);
	createLabirinth();
}

bool** Maze::getGameFieldArray()
{
	return game_field;
}

// —оздает начальное поле лабиринта.
void Maze::createLabirinthStart(int x_size, int y_size)
{
	for (int i = 0; i < y_size; i++)
	{
		for (int j = 0; j < x_size; j++)
		{
			game_field[i][j] = false;
		}
	}
}

// √енерирует лабиринт (динамический метод)
void Maze::createLabirinth()
{
	Vector2i start_pos(PLAYER_START_COORD_X, PLAYER_START_COORD_Y);

	game_field[start_pos.y][start_pos.x] = true;

	std::vector<sf::Vector2i> visited_points, neigbours;
	visited_points.push_back(start_pos);
	int random_start, random_neighbour;

	for (int i = 0; i < (y_size / 2) * (x_size / 2) - 1; i++)
	{
		while (neigbours.size() < 1)
		{
			random_start = GetRandNumb(0, visited_points.size() - 1);

			findNeigbours(neigbours, visited_points[random_start]); 
		}
		random_neighbour = GetRandNumb(0, neigbours.size() - 1);
		game_field[neigbours[random_neighbour].y][neigbours[random_neighbour].x] = true;
		removeWall(visited_points[random_start], neigbours[random_neighbour]);
		visited_points.push_back(neigbours[random_neighbour]);

		neigbours.clear();
	}
}

// ”дал€ет стену между двум€ сосед€ми.
void Maze::removeWall(Vector2i start, Vector2i finish)
{
	game_field[(start.y + finish.y) / 2][(start.x + finish.x) / 2] = true;
}

// Ќаходит "доступных" соседей дл€ клетки.
std::vector<Vector2i> Maze::findNeigbours(std::vector<Vector2i> &neigbours, Vector2i start_pos)
{
	for (int i = -2; i < 3; i += 2)
	{
		for (int j = -2; j < 3; j += 2)
		{
			if ((start_pos.y + i >= 0) && (start_pos.x + j >= 0) && (abs(i) != abs(j)) && (start_pos.y + i < y_size) && (start_pos.x + j < x_size))
			{
				if (game_field[start_pos.y + i][start_pos.x + j] != true)  // проверка на посещенность.
				{
					neigbours.push_back(Vector2i(start_pos.x + j, start_pos.y + i));
				}
			}
		}
	}
	return neigbours;
}