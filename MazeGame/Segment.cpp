#include "Segment.h"

Segment::Segment() {}

Segment::Segment(sf::Vector2f start, sf::Vector2f end)
{
	this->start_point = start;
	this->end_point = end;
}