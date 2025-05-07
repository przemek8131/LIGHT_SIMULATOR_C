/*
* @file cross.h
* @author Kapa³a Przemys³aw
* @date May 2025
* @brief File containing structures and function declarations for smart traffic light simulator.
*
*/

#ifndef CROSS_H
#define CROSS_H

#include<string.h>
#include<stdint.h>
#include<stdbool.h>

#define WAITING_STEPS (4)
#define NUM_OF_DIRECTIONS (4)

typedef enum {
	GREEN_LIGHT,
	YELLOW_LIGHT,
	RED_LIGHT
} light_state_t;

typedef enum {
	N = 0,
	S,
	E,
	W
} direction_t;

typedef enum {
	straight,
	left,
	right
} turn_t;

typedef struct {
	char* ID;
	direction_t end_direction;
	turn_t turn;
} vehicle_t;

typedef struct vehicle_node {
	vehicle_t vehicle;
	struct vehicle_node* next;
}vehicle_node_t;

typedef struct {
	vehicle_node_t* head;
	light_state_t main_light_state;
	light_state_t right_light_state; //strza³ka warunkowa w prawo
	uint8_t car_num;
	direction_t direction;
	uint8_t waiting_steps;
} road_t;

void road_initialize(road_t* road, direction_t direction);
void add_vehicle(road_t* roads, const char* ID, direction_t startRoad, direction_t endRoad);
void cross_step(road_t* roads, char** buff);
void send_left_cars(char** left_vehicle_buff);
#endif