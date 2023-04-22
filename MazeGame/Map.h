#pragma once

#include "Settings.h"
#include "Segment.h"
#include "Maze.h"

class Map
{
	std::vector<sf::Vertex> vertex_array;
	std::vector<Segment> segment_array;

	bool** vertical_walls;
	bool** horizontal_walls;
	bool** game_field;

	unsigned int x_size;
	unsigned int y_size;

	unsigned int current_level;
	sf::Vector2i exit_point;

	Maze* pMaze;

public:

	Map(const unsigned int size_x, const unsigned int size_y, int choise = 0);

	~Map();

	void update();

	bool** getVertLink();

	bool** getHorLink();

	bool** getMapLink();

	sf::Vector2i getMapSize();

	sf::Vector2i getExitPoint();

	std::vector<Segment>* getSegmentArrayLink();


private:

	void findWalls(bool** game_field);

	void findVertices(bool** game_field, const int x_size, const int y_size);

	void findSegments(bool** game_field);

};
