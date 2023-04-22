#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Settings.h"
#include "Mathematics.h"

using namespace sf;

Map::Map(const unsigned int size_x, const unsigned int size_y, int choise)
{
	this->x_size = size_x;
	this->y_size = size_y;

	pMaze = new Maze(x_size, y_size);

	game_field = pMaze->getGameFieldArray();

	delete pMaze;

	current_level = 1;
	exit_point = Vector2i(1 + GetRandNumb(0, 1) * (x_size - 3), 1 + GetRandNumb(0, 1) * (y_size - 3));

	switch (choise)
	{
	case 0:
		
		vertical_walls = new bool* [y_size];
		horizontal_walls = new bool* [y_size];

		for (int i = 0; i < y_size; i++)
		{
			vertical_walls[i] = new bool[x_size];
			horizontal_walls[i] = new bool[x_size];
			for (int j = 0; j < x_size; j++)
			{
				vertical_walls[i][j] = false;
				horizontal_walls[i][j] = false;
			}
		}
		findWalls(game_field);

		findVertices(game_field, x_size, y_size);
		findSegments(game_field);
		break;
	case 1:
		vertical_walls = new bool* [y_size];
		horizontal_walls = new bool* [y_size];

		for (int i = 0; i < y_size; i++)
		{
			vertical_walls[i] = new bool[x_size];
			horizontal_walls[i] = new bool[x_size];
			for (int j = 0; j < x_size; j++)
			{
				vertical_walls[i][j] = true;
				horizontal_walls[i][j] = true;
			}
		}
		break;
	default:
		break;
	}
}

Map::~Map()
{
	for (int i = 0; i < y_size; i++)
	{
		delete[] game_field[i];
		delete[] vertical_walls[i];
		delete[] horizontal_walls[i];
	}
	delete[] game_field;
	delete[] vertical_walls;
	delete[] horizontal_walls;
}

void Map::update()
{
	current_level++;

	for (int i = 0; i < y_size; i++)
	{
		delete[] game_field[i];
		delete[] vertical_walls[i];
		delete[] horizontal_walls[i];
	}
	delete[] game_field;
	delete[] vertical_walls;
	delete[] horizontal_walls;

	vertex_array.clear();
	segment_array.clear();

	if (current_level == 2)
	{
		x_size = y_size = SECOND_LEVEL_SIZE;
	}
	else
	{
		x_size = y_size = THIRD_LEVEL_SIZE;
	}
	pMaze = new Maze(x_size, y_size);
	game_field = pMaze->getGameFieldArray();
	delete pMaze;

	exit_point = Vector2i(1 + GetRandNumb(0, 1) * (x_size - 3), 1 + GetRandNumb(0, 1) * (y_size - 3));

	vertical_walls = new bool* [y_size];
	horizontal_walls = new bool* [y_size];

	for (int i = 0; i < y_size; i++)
	{
		vertical_walls[i] = new bool[x_size];
		horizontal_walls[i] = new bool[x_size];
		for (int j = 0; j < x_size; j++)
		{
			vertical_walls[i][j] = false;
			horizontal_walls[i][j] = false;
		}
	}

	findVertices(game_field, x_size, y_size);
	findWalls(game_field);
	findSegments(game_field);
}

bool** Map::getVertLink()
{
	return vertical_walls;
}

bool** Map::getHorLink()
{
	return horizontal_walls;
}

bool** Map::getMapLink()
{
	return game_field;
}

sf::Vector2i Map::getMapSize()
{
	return Vector2i(x_size, y_size);
}

Vector2i Map::getExitPoint()
{
	return exit_point;
}

std::vector<Segment>* Map::getSegmentArrayLink()
{
	return &segment_array;
}

void Map::findWalls(bool** game_field)
{
	for (int i = 0; i < y_size; i++)
	{
		for (int j = 0; j < x_size - 1; j++)
		{
			if (game_field[i][j] != game_field[i][j + 1])
			{
				vertical_walls[i][j] = true;
			}
		}
	}
	for (int i = 0; i < y_size - 1; i++)
	{
		for (int j = 0; j < x_size; j++)
		{
			if (game_field[i][j] != game_field[i + 1][j])
			{
				horizontal_walls[i][j] = true;
			}
		}
	}
}

void Map::findVertices(bool** game_field, const int x_size, const int y_size)
{
	unsigned short int counter;
	for (int y = 0; y < y_size - 1; y++)
	{
		for (int x = 0; x < x_size - 1; x++)
		{
			counter = 0;
			counter += game_field[y][x] + game_field[y][x + 1] + game_field[y + 1][x] + game_field[y + 1][x + 1];
			if (counter % 2 != 0)
			{
				vertex_array.push_back(Vertex(Vector2f((x + 1) * SQUARE_TILE, (y + 1) * SQUARE_TILE)));
			}
		}
	}
}

void Map::findSegments(bool** game_field)
{
	double min_distance_x, min_distance_y, cur_distance_x, cur_distance_y;
	int vertex_id_x, vertex_id_y, x_start, y_start, dx, dy;

	for (int i = 0; i < vertex_array.size(); i++)
	{
		min_distance_x = 200000, min_distance_y = 200000;
		vertex_id_x = -1, vertex_id_y = -1;
		y_start = (int)(vertex_array[i].position.y + 1) / (int)SQUARE_TILE;
		x_start = (int)(vertex_array[i].position.x + 1) / (int)SQUARE_TILE;

		for (int j = i + 1; j < vertex_array.size(); j++)
		{

			dx = Sign(vertex_array[j].position.x - vertex_array[i].position.x);
			dy = Sign(vertex_array[j].position.y - vertex_array[i].position.y);

			if (vertex_array[i].position.x == vertex_array[j].position.x &&
				game_field[(int)(vertex_array[i].position.y + dy) / (int)SQUARE_TILE][(int)(vertex_array[i].position.x) / (int)SQUARE_TILE] !=
				game_field[(int)(vertex_array[i].position.y + dy) / (int)SQUARE_TILE][(int)(vertex_array[i].position.x) / (int)SQUARE_TILE - 1])
			{
				cur_distance_y = VectorModule(vertex_array[i].position - vertex_array[j].position);
				if (cur_distance_y < min_distance_y)
				{
					min_distance_y = cur_distance_y;
					vertex_id_y = j;
				}
			}

			if (vertex_array[i].position.y == vertex_array[j].position.y &&
				game_field[(int)(vertex_array[i].position.y) / (int)SQUARE_TILE][(int)(vertex_array[i].position.x + dx) / (int)SQUARE_TILE] !=
				game_field[(int)(vertex_array[i].position.y) / (int)SQUARE_TILE - 1][(int)(vertex_array[i].position.x + dx) / (int)SQUARE_TILE])
			{
				cur_distance_x = VectorModule(vertex_array[i].position - vertex_array[j].position);
				if (cur_distance_x < min_distance_x)
				{
					min_distance_x = cur_distance_x;
					vertex_id_x = j;
				}
			}
		}

		Segment segment;
		segment.start_point = vertex_array[i].position;
		if (vertex_id_x != -1)
		{
			segment.end_point = vertex_array[vertex_id_x].position;
			segment_array.push_back(segment);
		}
		if (vertex_id_y != -1)
		{
			segment.end_point = vertex_array[vertex_id_y].position;
			segment_array.push_back(segment);
		}
	}
}

