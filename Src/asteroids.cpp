/*
 * asteroids.cpp
 *
 *  Created on: 22.12.2020
 *      Author: Julius
 *
 *	A game where you play a triangle that is angry at space rocks and decides to throw squares at them until they vanish.
 */

#include "asteroids.h"

#include <math.h>

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#include "stm32l1xx_hal.h"
#include "buttonmatrix.h"
#include "Display.h"
#include "gameconfig.h"

float deg_to_rad(int x){
	return x * M_PI / 180;
}

bool is_in_bound(float x, int min, int max){
	if (x < min or x > max){
		return false;
	}else{
		return true;
	}
}

int bound_check_int(int x, int min, int max){
	if (x < min){
		x = max - (min - x) + 1;
	}else if (x > max){
		x = min + (x - max) - 1;
	}
	return x;
}

float bound_check_float(float x, int min, int max){
	if (x < min){
		x = max - (min - x) + 1;
	}else if (x > max){
		x = min + (x - max) - 1;
	}
	return x;
}

//#####################################################################

const std::vector<std::vector<int>> Ship::offsetN = {
	{2, 0},
	{1, 1},
	{2, 1},
	{3, 1},
	{1, 2},
	{2, 2},
	{3, 2},
	{0, 3},
	{1, 3},
	{2, 3},
	{3, 3},
	{4, 3},
	{0, 4},
	{1, 4},
	{2, 4},
	{3, 4},
	{4, 4}
};

const std::vector<std::vector<int>> Ship::offsetNE = {
	{1, 0},
	{2, 0},
	{3, 0},
	{4, 0},
	{0, 1},
	{1, 1},
	{2, 1},
	{3, 1},
	{4, 1},
	{1, 2},
	{2, 2},
	{3, 2},
	{4, 2},
	{2, 3},
	{3, 3},
	{4, 3},
	{3, 4}
};

const std::vector<std::vector<int>> Ship::offsetE = {
	{0, 0},
	{1, 0},
	{0, 1},
	{1, 1},
	{2, 1},
	{3, 1},
	{0, 2},
	{1, 2},
	{2, 2},
	{3, 2},
	{4, 2},
	{0, 3},
	{1, 3},
	{2, 3},
	{3, 3},
	{0, 4},
	{1, 4}
};

const std::vector<std::vector<int>> Ship::offsetSE = {
	{3, 0},
	{2, 1},
	{3, 1},
	{4, 1},
	{1, 2},
	{2, 2},
	{3, 2},
	{4, 2},
	{0, 3},
	{1, 3},
	{2, 3},
	{3, 3},
	{4, 3},
	{1, 4},
	{2, 4},
	{3, 4},
	{4, 4}
};

const std::vector<std::vector<int>> Ship::offsetS = {
	{0, 0},
	{1, 0},
	{2, 0},
	{3, 0},
	{4, 0},
	{0, 1},
	{1, 1},
	{2, 1},
	{3, 1},
	{4, 1},
	{1, 2},
	{2, 2},
	{3, 2},
	{1, 3},
	{2, 3},
	{3, 3},
	{2, 4}
};

const std::vector<std::vector<int>> Ship::offsetSW = {
	{1, 0},
	{0, 1},
	{1, 1},
	{2, 1},
	{0, 2},
	{1, 2},
	{2, 2},
	{3, 2},
	{0, 3},
	{1, 3},
	{2, 3},
	{3, 3},
	{4, 3},
	{0, 4},
	{1, 4},
	{2, 4},
	{3, 4}
};

const std::vector<std::vector<int>> Ship::offsetW = {
	{3, 0},
	{4, 0},
	{1, 1},
	{2, 1},
	{3, 1},
	{4, 1},
	{0, 2},
	{1, 2},
	{2, 2},
	{3, 2},
	{4, 2},
	{1, 3},
	{2, 3},
	{3, 3},
	{4, 3},
	{3, 4},
	{4, 4}
};

const std::vector<std::vector<int>> Ship::offsetNW = {
	{0, 0},
	{1, 0},
	{2, 0},
	{3, 0},
	{0, 1},
	{1, 1},
	{2, 1},
	{3, 1},
	{4, 1},
	{0, 2},
	{1, 2},
	{2, 2},
	{3, 2},
	{0, 3},
	{1, 3},
	{2, 3},
	{1, 4}
};

Ship::Ship(int new_angle, int new_position_x, int new_position_y)
			:angle(new_angle), position_x(new_position_x), position_y(new_position_y){
	orientate();
}

Ship::~Ship(void){

}

void Ship::orientate(void){
//Adjusts ship orientation on display according to current angle.
	switch(angle){
	case 24 ... 68: //NE
		orientation = 1;
		break;
	case 69 ... 113: //N
		orientation = 0;
		break;
	case 114 ... 158: //NW
		orientation = 7;
		break;
	case 159 ... 203: //W
		orientation = 6;
		break;
	case 204 ... 248: //SW
		orientation = 5;
		break;
	case 249 ... 293: //S
		orientation = 4;
		break;
	case 294 ... 338: //SE
		orientation = 3;
		break;
	default: //E
		orientation = 2;
	}
}

void Ship::move(void){
//Moves ship using velocity multiplier according to its heading (angle).
//If the boundaries of the display are exceeded, the ship will reenter the display on the opposite side.
	position_x = bound_check_float(position_x + velocity * cos(deg_to_rad(angle)), 0, Asteroids::disp_bound_x);
	position_y = bound_check_float(position_y + velocity * -sin(deg_to_rad(angle)), 0, Asteroids::disp_bound_y);
}

void Ship::rotate_cw(void){
//Rotates the ship clockwise.
	angle = bound_check_int(angle - angle_velocity, 0, 359);
	orientate();
}

void Ship::rotate_ccw(void){
//Rotates the ship counterclockwise.
	angle = bound_check_int(angle + angle_velocity, 0, 359);
	orientate();
}

std::vector<std::vector<int>> Ship::get_hitbox(void){
//Returns a vector of coordinates for each pixel of the ship on the display.
	std::vector<std::vector<int>> offset;
	offset.resize(offsetN.size(), std::vector<int>(2, 0));
	switch(orientation){
	case 0: //N
		offset = offsetN;
		break;
	case 1: //NE
		offset = offsetNE;
		break;
	case 2: //E
		offset = offsetE;
		break;
	case 3: //SE
		offset = offsetSE;
		break;
	case 4: //S
		offset = offsetS;
		break;
	case 5: //SW
		offset = offsetSW;
		break;
	case 6: //W
		offset = offsetW;
		break;
	case 7: //NW
		offset = offsetNW;
		break;
	default:
		offset = offsetN;
	}
	std::vector<std::vector<int>> hitbox;
	hitbox.resize(offset.size(), std::vector<int>(2, 0));
	for(int i = 0; i < (int)offset.size(); i++) {
		hitbox[i][0] = bound_check_int(static_cast<int>(position_x) + offset[i][0], 0, Asteroids::disp_bound_x);
		hitbox[i][1] = bound_check_int(static_cast<int>(position_y) + offset[i][1], 0, Asteroids::disp_bound_y);
	}
	return hitbox;
}

int Ship::get_angle(void){
	return angle;
}

int Ship::get_position_x(void){
	return (int)position_x;
}

int Ship::get_position_y(void){
	return (int)position_y;
}

//#####################################################################

Bullet::Bullet(int new_angle, int new_position_x, int new_position_y)
				:angle(new_angle), position_x(new_position_x), position_y(new_position_y){
}

Bullet::~Bullet(void){

}

bool Bullet::move(void){
//Moves bullet using velocity multiplier according to its heading (angle).
//Returns TRUE if movement is valid and FALSE if display boundaries are exceeded.
	position_x = position_x + velocity * cos(deg_to_rad(angle));
	position_y = position_y + velocity * -sin(deg_to_rad(angle));
	if(not is_in_bound(position_x, 0, Asteroids::disp_bound_x)
	or not is_in_bound(position_y, 0, Asteroids::disp_bound_y)){
		return false;
	}else{
		return true;
	}
}

std::vector<std::vector<int>> Bullet::get_hitbox(void){
//Returns a vector with the coordinate of the bullet. A two-dimensional vector is used to maintain consistency with get_hitbox() functions of other classes.
	std::vector<std::vector<int>> hitbox;
	hitbox.resize(1, std::vector<int>(2, 0));
	hitbox[0][0] = bound_check_int(static_cast<int>(position_x), 0, Asteroids::disp_bound_x);
	hitbox[0][1] = bound_check_int(static_cast<int>(position_y), 0, Asteroids::disp_bound_y);
	return hitbox;
}

int Bullet::get_angle(void){
	return angle;
}

int Bullet::get_velocity(void){
	return velocity;
}

//#####################################################################

const std::vector<std::vector<int>> Rock::offset = {
		{2, 1},
		{3, 1},
		{5, 1},
		{1, 2},
		{2, 2},
		{3, 2},
		{4, 2},
		{5, 2},
		{6, 2},
		{0, 3},
		{1, 3},
		{2, 3},
		{3, 3},
		{4, 3},
		{5, 3},
		{6, 3},
		{0, 4},
		{1, 4},
		{2, 4},
		{3, 4},
		{4, 4},
		{5, 4},
		{0, 5},
		{1, 5},
		{2, 5},
		{3, 5},
		{4, 5},
		{5, 5},
		{6, 5},
		{1, 6},
		{2, 6},
		{3, 6},
		{4, 6},
		{5, 6},
		{6, 6},
		{2, 7},
		{3, 7},
		{4, 7}
};

Rock::Rock(int new_angle, int new_position_x, int new_position_y){
	angle = new_angle;
	position_x = new_position_x;
	position_y = new_position_y;
}

Rock::~Rock(void){

}

void Rock::move(void){
//Moves rock using velocity multiplier according to its heading (angle).
//If the boundaries of the display are exceeded, the rock will reenter the display on the opposite side.
	position_x = bound_check_float(position_x + velocity * cos(deg_to_rad(angle)), 0, Asteroids::disp_bound_x);
	position_y = bound_check_float(position_y + velocity * -sin(deg_to_rad(angle)), 0, Asteroids::disp_bound_y);
}

std::vector<std::vector<int>> Rock::get_hitbox(void){
//Returns a vector of coordinates for each pixel of the rock on the display.
	std::vector<std::vector<int>> shape;
	shape.resize(offset.size(), std::vector<int>(2, 0));
	for(int i = 0; i < (int)offset.size(); i++){
		shape[i][0] = bound_check_int(static_cast<int>(position_x) + offset[i][0], 0, Asteroids::disp_bound_x);
		shape[i][1] = bound_check_int(static_cast<int>(position_y) + offset[i][1], 0, Asteroids::disp_bound_y);
	}
	return shape;
}

//#####################################################################

Asteroids::Asteroids(Display& new_disp, Gameconfig& new_gameconfig): Spiel(std::string("asteroiden"))
	, disp(new_disp), gameconfig(new_gameconfig){
		
	//waves_to_win = 3;
	waves_to_win= gameconfig.get("aswaves");
}

Asteroids::~Asteroids(void){

}

void Asteroids::draw(std::vector<std::vector<int>> hitbox){
//Draws object on display on a pixel-by-pixel basis.
	for(int i = 0; i < (int)hitbox.size(); i++){
		disp.drawpixel(hitbox[i][0], hitbox[i][1]);
	}
}

void Asteroids::run(void){
	//Reset the wave count.
	wave_cnt = 0;
	//Create random and ship objects.
	Ship myShip(90, disp_bound_x / 2 - 2, disp_bound_y / 2 - 2);
	//Display the start screen.
	disp.cleanbuffer();
	draw(myShip.get_hitbox());
	disp.setfont(1);
	disp.writestring(disp_bound_x / 2 - 27, disp_bound_y / 3 + 2, "PRESS A BUTTON");
	disp.writestring(disp_bound_x / 2 - 13, disp_bound_y / 3 * 2 + 2, "TO PLAY");
	disp.updatescreen();
	//Wait for a player input to start the game.
	while(getButton() == 0){
		HAL_Delay(100);
	}
	static std::mt19937 mt(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	
	while(true){
		//Start a new wave if no rocks are left.
		if(rocks.empty()){
			//Create random distributions used to parametrize new rocks.
			std::uniform_int_distribution<int> dist_angle(0, 359);
			std::uniform_int_distribution<int> dist_position_x(myShip.get_hitbox()[0][0] + 20, myShip.get_hitbox()[0][0] + disp_bound_x - 40);
			std::uniform_int_distribution<int> dist_position_y(myShip.get_hitbox()[0][1] + 20, myShip.get_hitbox()[0][1] + disp_bound_y - 40);
			wave_cnt++;
			//Spawn new rocks.
			for(int i = 0; i < rocks_per_wave; i++){
				rocks.push_back({dist_angle(mt), bound_check_int(dist_position_x(mt), 0, disp_bound_x), bound_check_int(dist_position_y(mt), 0, disp_bound_y)});
			}
		}
		//Check the winning condition.
		if(wave_cnt > waves_to_win){
			//Clear all objects.
	    	rocks.clear();
	    	bullets.clear();
	    	myShip.~Ship();
			//Display the win screen.
			disp.cleanbuffer();
			disp.setfont(2);
			disp.writestring(disp_bound_x / 2 - 39, disp_bound_y / 2 + 4, "YOU WIN!");
			disp.updatescreen();
			//Wait 3 seconds then exit the run() function.
			HAL_Delay(3000);
			break;
		}
		//Move the bullets.
		auto i_bullet = std::begin(bullets);
		while (i_bullet != std::end(bullets)){
			if(not i_bullet->move()){
				//Erase a bullet if it is out of bounds.
				i_bullet = bullets.erase(i_bullet);
			}else{
				i_bullet++;
			}
		}
		//Move the rocks and check for collisions with the ship or bullets.
		ship_hit = false;
		auto i_rock = std::begin(rocks);
		while (i_rock != std::end(rocks)){
			rock_hit = false;
	    	i_rock->move();
			for(const auto& point_rock: i_rock->get_hitbox()){
				for(const auto& point_ship: myShip.get_hitbox()){
					//Check for a collision between a rock and the ship.
				    if(point_rock == point_ship){
				    	ship_hit = true;
				    }
				}
				auto i_bullet = std::begin(bullets);
				while (i_bullet != std::end(bullets)){
					//Check for a collision between a rock and a bullet.
					if(point_rock == i_bullet->get_hitbox()[0]){
						//Erase the bullet on collision with the rock.
						i_bullet = bullets.erase(i_bullet);
						rock_hit = true;
					}else{
				        i_bullet++;
					}
				}
			}

			if(rock_hit){
				//Erase the rock on collision with a bullet.
				i_rock = rocks.erase(i_rock);
			}else{
		        i_rock++;
			}
	    }
		//Check the losing condition.
		if(ship_hit){
	    	//Clear all rocks and bullets.
	    	rocks.clear();
	    	bullets.clear();
	    	wave_cnt = 0;
	    	//Display the lose screen.
			disp.cleanbuffer();
			draw(myShip.get_hitbox());
			disp.setfont(1);
			disp.writestring(bound_check_int(myShip.get_hitbox()[0][0] - 17, 0, disp_bound_x), bound_check_int(myShip.get_hitbox()[0][1] - 7, 0, disp_bound_y), "YOU LOSE!");
			disp.updatescreen();
			//Wait 1 second then continue the game with a reset wave counter and new rocks.
			HAL_Delay(1000);
		}
		//Check for player inputs.
		switch(getButton()){
		case 1:
			//The ship rotates counterclockwise.
			myShip.rotate_ccw();
			break;
		case 2:
			//The ship rotates clockwise.
			myShip.rotate_cw();
			break;
		case 3:
			//Thrust is applied to the ship.
			myShip.move();
			break;
		case 4:
			//A bullet is fired from the ship.
			bullets.push_back({myShip.get_angle(), myShip.get_position_x() + 2, myShip.get_position_y() + 2});
		}
		//Update the display.
		disp.cleanbuffer();
		draw(myShip.get_hitbox());
		for(auto& rock : rocks){
			draw(rock.get_hitbox());
		}
		for(auto& bullet : bullets){
			draw(bullet.get_hitbox());
		}
		disp.updatescreen();
	}
}

void Asteroids::set_waves_to_win(int new_waves_to_win){
	waves_to_win = new_waves_to_win;
	waves_to_win= gameconfig.get("aswaves");
	
}
