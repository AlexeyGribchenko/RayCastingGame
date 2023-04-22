#include <random>
#include <SFML/Graphics.hpp>
#include "Mathematics.h"

int Sign(float value)
{
	return (value > 0) ? 1 : (value == 0) ? 0 : -1;
}

float PseudoScalarProduct(sf::Vector2f firsf_vec, sf::Vector2f second_vec)
{
	return firsf_vec.x * second_vec.y - second_vec.x * firsf_vec.y;
}

float VectorModule(sf::Vector2f vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

int GetRandNumb(unsigned int start, unsigned int end)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(start, end);
	return dist(gen);
}