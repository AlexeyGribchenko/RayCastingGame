#pragma once

class Maze
{
private:

	int x_size;
	int y_size;

	bool** game_field;

	// —оздает начальное поле лабиринта.
	void createLabirinthStart(int x_size, int y_size);

	// √енерирует лабиринт (динамический метод)
	void createLabirinth();

	// ”дал€ет стену между двум€ сосед€ми.
	void removeWall(sf::Vector2i start, sf::Vector2i finish);

	// Ќаходит "доступных" соседей дл€ клетки.
	std::vector<sf::Vector2i> findNeigbours(std::vector<sf::Vector2i> &neigbours, sf::Vector2i start_pos);


public:

	Maze(int x_size, int y_size);

	bool** getGameFieldArray();

};