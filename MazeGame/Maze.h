#pragma once

class Maze
{
private:

	int x_size;
	int y_size;

	bool** game_field;

	// ������� ��������� ���� ���������.
	void createLabirinthStart(int x_size, int y_size);

	// ���������� �������� (������������ �����)
	void createLabirinth();

	// ������� ����� ����� ����� ��������.
	void removeWall(sf::Vector2i start, sf::Vector2i finish);

	// ������� "���������" ������� ��� ������.
	std::vector<sf::Vector2i> findNeigbours(std::vector<sf::Vector2i> &neigbours, sf::Vector2i start_pos);


public:

	Maze(int x_size, int y_size);

	bool** getGameFieldArray();

};