#include <SFML/Graphics.hpp>
#include "RayCasting.h"
#include "Settings.h"
#include "Mathematics.h"
#include <iostream>

using namespace sf;

void RayCasting::drawScene(RenderWindow& window, Player& player, Map& world_map, Texture& texture)
{
	float Px, Py,
		distance,
		dist_v, dist_h,
		color_factor,
		sin_a, cos_a,
		pl_x, pl_y,
		dx, dy;

	float horizont_pos = player.getViewVertPos();
	float pl_angle = player.getAngle();
	Vector2f pl_pos = player.getPosition();

	bool** vert_walls = world_map.getVertLink();
	bool** hor_walls = world_map.getHorLink();
	//---------------Floor and sky drawing----------------
	drawFloorAndSky(window, horizont_pos);

	pl_x = (int)(pl_pos.x / SQUARE_TILE) * SQUARE_TILE;
	pl_y = (int)(pl_pos.y / SQUARE_TILE) * SQUARE_TILE;

	for (float i_angle = -H_FOV; i_angle < H_FOV; i_angle += DELTA_ANGLE)
	{
		float offset;

		sin_a = sin(pl_angle + i_angle);
		cos_a = cos(pl_angle + i_angle);

		if (sin_a == 0) { sin_a += 0.00001; }
		if (cos_a == 0) { cos_a += 0.00001; }

		// Vertical wall processing.
		if (cos_a >= 0) {
			Px = pl_x + SQUARE_TILE;
			dx = 1;
		}
		else {
			Px = pl_x;
			dx = -1;
		}

		for (int j = (int)(pl_x / SQUARE_TILE); j < world_map.getMapSize().x && j >= 0; j += dx)
		{
			dist_v = (Px - pl_pos.x) / cos_a;
			Py = pl_pos.y + dist_v * sin_a;
			if (Py < 0 || (int)(Py / SQUARE_TILE) > world_map.getMapSize().y - 1)
			{
				dist_v = MAX_RAY_LENGHT;
				break;
			}
			else if (vert_walls[(int)(Py / SQUARE_TILE)][(int)((Px - 1) / SQUARE_TILE)])
			{
				break;
			}
			Px += dx * SQUARE_TILE;
		}
		offset = Py;

		// Horizontal wall processing.
		if (sin_a >= 0)
		{
			Py = pl_y + SQUARE_TILE;
			dy = 1;
		}
		else
		{
			Py = pl_y;
			dy = -1;
		}

		for (int j = pl_y / SQUARE_TILE; j < world_map.getMapSize().y && j >= 0; j += dy)
		{
			dist_h = (Py - pl_pos.y) / sin_a;
			Px = pl_pos.x + dist_h * cos_a;
			if (Px < 0 || (int)(Px / SQUARE_TILE) > world_map.getMapSize().x - 1)
			{
				dist_h = MAX_RAY_LENGHT;
				break;
			}
			else if (hor_walls[(int)((Py - 1) / SQUARE_TILE)][(int)(Px / SQUARE_TILE)])
			{
				break;
			}
			Py += dy * SQUARE_TILE;
		}

		if (dist_h < dist_v)
		{
			distance = dist_h;
			offset = Px;
		}
		else
		{
			distance = dist_v;
		}

		distance *= cos(i_angle);	 // Fixing fish-eye bug
		offset = fmod(offset, SQUARE_TILE);  // Offset processing (for texturing)
		offset /= SQUARE_TILE;

		//-----------------------------Wall drawing-------------------------------
		drawWall(window, i_angle, distance, offset, texture, player.getViewVertPos());
	}
}

void RayCasting::screen2(RenderWindow& window, Player& player, Map& world_map)
{
	RectangleShape rect;

	Vector2f pl_pos = player.getPosition();
	float pl_angle = player.getAngle();
	
	bool** vert_walls = world_map.getVertLink();
	bool** hor_walls = world_map.getHorLink();

	Texture wall_texture;
	wall_texture.loadFromFile("brick_wall.png");

	drawFloorAndSky(window, player.getViewVertPos());

	for (float i_angle = -H_FOV; i_angle < H_FOV; i_angle += DELTA_ANGLE)
	{
		float distance, offset;
		float sin_a = sin(pl_angle + i_angle);
		float cos_a = cos(pl_angle + i_angle);

		distance = MAX_RAY_LENGHT;
		offset = 0;

		for (float depth = 0; depth < MAX_RAY_LENGHT; depth+=0.5)
		{
			float x = pl_pos.x + depth * cos_a;
			float y = pl_pos.y + depth * sin_a;

			if (!world_map.getMapLink()[(int)(y / SQUARE_TILE)][(int)(x / SQUARE_TILE)])
			{
				if ((int)x % (int)SQUARE_TILE < 10)
				{
					offset = fmod(y, SQUARE_TILE);
				}
				else
				{
					offset = fmod(x, SQUARE_TILE);
				}
				distance = depth;
				break;
			}
		}
		//std::cout << distance << '\n';
		distance *= cos(i_angle);
		offset /= SQUARE_TILE;



		float height_ratio = 1 - ((DISTANCE_TO_DISPLAY * TILE_H) / (distance)) / DISTANCE_TO_DISPLAY;
		float height_projection = HEIGHT * (1 - height_ratio);
		float h_scale = height_projection / TILE_H;
		float color_factor = 255 / (1 + distance * distance * COLOR_FACTOR_RATIO);

		Sprite wall_sprite;
		wall_sprite.setTexture(wall_texture);
		wall_sprite.setTextureRect(IntRect(offset * wall_texture.getSize().x, 0, 1, wall_texture.getSize().x));
		wall_sprite.setScale(TILE_W, h_scale);
		wall_sprite.setPosition(Vector2f(TILE_W * ((i_angle + H_FOV) / DELTA_ANGLE), HEIGHT / 2 * height_ratio + player.getViewVertPos()));
		wall_sprite.setColor(Color(color_factor, color_factor, color_factor));
		window.draw(wall_sprite);

		//window.draw(rect);
	}

}

void RayCasting::screen3(RenderWindow& window, Player& player, Map& world_map)
{
	drawFloorAndSky(window, player.getViewVertPos());

	Vector2f pl_pos = player.getPosition();
	float pl_angle = player.getAngle();

	for (float i_angle = -H_FOV; i_angle < H_FOV; i_angle += DELTA_ANGLE)
	{
		// определяем конечную точку луча
		Vector2f end_coord = Vector2f(pl_pos.x + MAX_RAY_LENGHT * cos(i_angle + pl_angle), pl_pos.y + MAX_RAY_LENGHT * sin(i_angle + pl_angle));
		Segment ray(pl_pos, end_coord);

		std::vector<Segment>* segments = world_map.getSegmentArrayLink();

		float min_dist = MAX_RAY_LENGHT;

		for (int i = 0; i < (*segments).size(); i++)
		{
			float dist = getDistance(ray, (*segments)[i]);
			if (dist < min_dist)
			{
				min_dist = dist;
			}
		}

		float color_factor = ((MAX_RAY_LENGHT - min_dist)) / (MAX_RAY_LENGHT);

		RectangleShape wall;
		wall.setFillColor(Color(255 * color_factor, 153 * color_factor, 51 * color_factor));
		min_dist *= cos(i_angle);		 // Fixing fish-eye bug
		wall.setSize(Vector2f(TILE_W, TILE_H * HEIGHT / (min_dist)));
		wall.setPosition(Vector2f(TILE_W * ((i_angle + H_FOV) / (H_FOV * 2 / NUM_RAYS)), HEIGHT / 2 - TILE_H * HEIGHT / (min_dist * 2) + player.getViewVertPos()));

		window.draw(wall);
	}
}

float RayCasting::getDistance(Segment ray, Segment segment)
{
	double min_distance = MAX_RAY_LENGHT, cur_distance;

	Vector2f cross_point;

	Vector2f ray_vec = ray.end_point - ray.start_point;
	Vector2f cur_vec = segment.end_point - segment.start_point;

	Vector2f vec1 = segment.start_point - ray.start_point;
	Vector2f vec2 = segment.end_point - ray.start_point;

	Vector2f vec3 = ray.end_point - segment.start_point;
	Vector2f vec4 = ray.start_point - segment.start_point;

	double prod1 = PseudoScalarProduct(ray_vec, vec1);
	double prod2 = PseudoScalarProduct(ray_vec, vec2);

	double prod3 = PseudoScalarProduct(cur_vec, vec3);
	double prod4 = PseudoScalarProduct(cur_vec, vec4);

	if ((Sign(prod1) != Sign(prod2) || prod1 == 0 || prod2 == 0) && (Sign(prod3) != Sign(prod4) || prod3 == 0 || prod4 == 0))
	{
		cross_point.x = segment.start_point.x + (segment.end_point.x - segment.start_point.x) * (abs(prod1) / abs(prod2 - prod1));
		cross_point.y = segment.start_point.y + (segment.end_point.y - segment.start_point.y) * (abs(prod1) / abs(prod2 - prod1));
		cur_distance = sqrt(pow(cross_point.x - ray.start_point.x, 2) + pow(cross_point.y - ray.start_point.y, 2));
	}
	else
	{
		cur_distance = MAX_RAY_LENGHT;
	}
	if (cur_distance < min_distance)
	{
		min_distance = cur_distance;
	}
	return min_distance;
}

void RayCasting::drawWall(RenderWindow& window, float i_angle, float distance, float offset, Texture& texture, float horizont_pos)
{
	float texture_size = texture.getSize().x;
	float height_ratio = 1 - ((TILE_H) / (distance));
	float height_projection = HEIGHT * (TILE_H / distance);
	float h_scale = height_projection / texture_size;
	Sprite wall_sprite;

	float color_factor = 255 / (1 + distance * distance * COLOR_FACTOR_RATIO);
	wall_sprite.setTexture(texture);
	wall_sprite.setTextureRect(IntRect(offset * texture_size, 0, 1, texture_size));
	wall_sprite.setScale(TILE_W, h_scale);
	wall_sprite.setPosition(Vector2f(TILE_W * ((i_angle + H_FOV) / DELTA_ANGLE), HEIGHT / 2 * height_ratio + horizont_pos));
	wall_sprite.setColor(Color(color_factor, color_factor, color_factor));
	window.draw(wall_sprite);
}

void RayCasting::drawFloorAndSky(RenderWindow& window, float horizont_pos)
{
	RectangleShape floor, sky;

	sky.setFillColor(Color(0, 204, 255));
	sky.setSize(Vector2f(WIDTH, HEIGHT / 2 + horizont_pos));
	sky.setPosition(Vector2f(0, 0));
	window.draw(sky);

	for (int i = HEIGHT / 2; i <= HEIGHT - horizont_pos; i++)
	{
		float dist = TILE_H / (1 - 2 * i / HEIGHT) - DISTANCE_TO_DISPLAY / 2;
		float color_factor = 1 / (1 + dist * dist * COLOR_FACTOR_RATIO);
		floor.setFillColor(Color(60 * color_factor, 60 * color_factor, 60 * color_factor));
		floor.setSize(Vector2f(WIDTH, 1));
		floor.setPosition(Vector2f(0, i + horizont_pos));
		window.draw(floor);
	}
}