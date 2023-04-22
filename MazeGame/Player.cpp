#include <SFML/Graphics.hpp>
#include "Player.h"


using namespace sf;

Player::Player()
{
	position.x = PLAYER_START_COORD_X * SQUARE_TILE;
	position.y = PLAYER_START_COORD_Y * SQUARE_TILE;

	angle = PLAYER_START_ANGLE;
	view_vert_pos = PLAYER_START_VIEW_VER_POS;
	max_speed = MAX_PLAYER_SPEED;
	speed = PLAYER_START_SPEED;
}

void Player::update(RenderWindow& window, Map& map)
{
	bool **game_field = map.getMapLink();
	float dx = 0, dy = 0;
	bool moved = false;

	if (Keyboard::isKeyPressed(Keyboard::LShift)){ max_speed = MAX_SHIFT_PLAYER_SPEED; }
	else { max_speed = MAX_PLAYER_SPEED; }
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::D))
		{ speed = max_speed / sqrt(2); }
		else { speed = max_speed; }
		dy += sin(angle) * speed;
		dx += cos(angle) * speed;
		moved = true;
	}
	if (Keyboard::isKeyPressed(Keyboard::S))
	{
		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::D))
		{ speed = max_speed / sqrt(2); }
		else { max_speed = MAX_PLAYER_SPEED; }
		dy -= sin(angle) * speed;
		dx -= cos(angle) * speed;
		moved = true;
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::S)) { speed = max_speed / sqrt(2); }
		else { speed = max_speed; }
		dy -= cos(angle) * speed;
		dx += sin(angle) * speed;
		moved = true;
	}
	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::S)) { speed = max_speed / sqrt(2); }
		else { speed = max_speed; }
		dy += cos(angle) * speed;
		dx -= sin(angle) * speed;
		moved = true;
	}

	if (moved)
	{
		if (game_field[(int)position.y / (int)SQUARE_TILE][(int)(position.x + dx * 10) / (int)SQUARE_TILE])
		{
			position.x += dx;
		}
		if (game_field[(int)(position.y + dy * 10) / (int)SQUARE_TILE][(int)position.x / (int)SQUARE_TILE])
		{
			position.y += dy;
		}
	}

	if ((Vector2i)(this->position / SQUARE_TILE) == map.getExitPoint())
	{
		map.update();
		setPosition((Vector2f)(map.getMapSize() / 2 + Vector2i(1, 1)) * SQUARE_TILE + Vector2f(SQUARE_TILE / 2, SQUARE_TILE / 2)); // (Vector2f)(map.getMapSize() / 2 + 1) * SQUARE_TILE
	}

	// ѕроверка: находитс€ ли курсор внутри окна.
	if (window.hasFocus())
	{
		angle += (Mouse::getPosition(window).x - WIDTH / 2) * 0.0006;
		angle += (Keyboard::isKeyPressed(Keyboard::Right)) ? 0.06 : (Keyboard::isKeyPressed(Keyboard::Left)) ? -0.06 : 0;
		view_vert_pos -= (Mouse::getPosition(window).y - HEIGHT / 2) * 0.9;
		(view_vert_pos > HEIGHT + 2400) ? view_vert_pos = HEIGHT + 2400 : view_vert_pos = view_vert_pos;
		(view_vert_pos < -HEIGHT - 2400) ? view_vert_pos = -HEIGHT - 2400 : view_vert_pos = view_vert_pos;
		Mouse::setPosition(Vector2i(WIDTH / 2, HEIGHT / 2), window);
	}
}

float Player::getAngle()
{
	return angle;
}

float Player::getViewVertPos()
{
	return view_vert_pos;
}

Vector2f Player::getPosition()
{
	return position;
}

void Player::setPosition(Vector2f new_position)
{
	this->position = new_position;
}