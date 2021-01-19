/*
 * asteroids.h
 *
 *  Created on: 22.12.2020
 *      Author: Julius
 */

#ifndef SRC_ASTEROIDS_H_
#define SRC_ASTEROIDS_H_

#include <vector>

#include "Display.h"
#include "spiel.h"

class Ship{
//a player controlled spaceship
	public:
	Ship(int new_angle, int new_Position_x, int new_Position_y);
	virtual ~Ship(void);

	void orientate(void);
	void move(void);
	void rotate_cw(void);
	void rotate_ccw(void);

	std::vector<std::vector<int>> get_hitbox(void);
	int get_angle(void);
	int get_position_x(void);
	int get_position_y(void);
private:
	static const int velocity = 5; //speed multiplier in pixels
	static const int angle_velocity = 20; //rotational speed multiplier in degrees

	static const std::vector<std::vector<int>> offsetN; //vector of coordinate offsets from position, to draw image of ship on display facing north
	static const std::vector<std::vector<int>> offsetNE; //...north east
	static const std::vector<std::vector<int>> offsetE; //...east
	static const std::vector<std::vector<int>> offsetSE; //...south east
	static const std::vector<std::vector<int>> offsetS; //...south
	static const std::vector<std::vector<int>> offsetSW; //...south west
	static const std::vector<std::vector<int>> offsetW; //...west
	static const std::vector<std::vector<int>> offsetNW; //...north west

	int orientation; //0:N, 1:NE, 2:E, 3:SE, 4:S, 5:SW, 6:W, 7:NW
	int angle; //direction of movement in 0-359 degree
	float position_x; //x coordinates in pixels
	float position_y; //y coordinates in pixels
};

class Bullet{
//the projectiles fired from the ship
public:
	Bullet(int new_angle, int new_position_x, int new_position_y);
	virtual ~Bullet(void);

	bool move(void);

	std::vector<std::vector<int>> get_hitbox(void);
	int get_angle(void);
	int get_velocity(void);
private:
	static const int velocity = 6; //speed multiplier in pixels

	int angle; //direction of movement in 0-359 degree
	float position_x; //x coordinates in pixels
	float position_y; //y coordinates in pixels
};

class Rock{
//the space rocks aka asteroids floating around
public:
	Rock(int new_angle, int new_position_x, int new_position_y);
	virtual ~Rock(void);

	void move(void);

	std::vector<std::vector<int>> get_hitbox(void);
private:
	static const int velocity = 3; //speed multiplier in pixels

	static const std::vector<std::vector<int>> offset; //vector of coordinate offsets from position, to draw image of rock on display

	int angle; //direction of movement in 0-359 degree
	float position_x; //x coordinates in pixels
	float position_y; //y coordinates in pixels
};

class Asteroids : public Spiel{ 

  public: 
	static const int disp_bound_x = 127; //display boundaries or pixel count
	static const int disp_bound_y = 63;

	Asteroids(Display& new_disp);
	virtual ~Asteroids(void);

	void run(void) override; 

	void set_waves_to_win(int new_waves_to_win);
private:
	static const int rocks_per_wave = 3; //number of rocks being spawned per wave

	Display& disp;

	std::vector<Rock> rocks; //vector containing dynamically created rock objects
	std::vector<Bullet> bullets; //vector containing dynamically created bullet objects
	int waves_to_win; //number of waves cleared needed to win
	int wave_cnt; //current wave counter
	bool rock_hit; //temporary flag set on collision between a rock and a bullet
	bool ship_hit; //temporary flag set on collision between the ship and a rock

	void draw(std::vector<std::vector<int>> hitbox);
};

#endif /* SRC_ASTEROIDS_H_ */
