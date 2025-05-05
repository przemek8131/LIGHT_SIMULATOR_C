#ifndef CROSS_H
#define CROSS_H

#include<string.h>
#include<stdint.h>
#include<stdbool.h>

#define VEHICLES_SIZE (10)
#define VEHICLES_ID_SIZE (10) //Vehicle1 - Vehicle99 + \0
#define NUM_OF_ROADS (4)
//#define NUM_OF_LINES (1)
//TODO: powinien byæ jeszcze RED_YELLOW_LIGHT, jako ¿ó³te po czerwonym
typedef enum {GREEN_LIGHT,
			  YELLOW_LIGHT, 
			  RED_LIGHT
} light_state_t;


//byæ mo¿e lepiej zrobiæ po prostu char
typedef enum {N,
			  S,
			  E,
			  W
} direction_t;

typedef enum {straight,
			  left,
			  right
} turn_t;

typedef struct {
	char* ID;
	//TODO: trzeba zmienic na string
	//direction_t startRoad;
	direction_t end_direction;
	turn_t turn;
} vehicle_t;

typedef struct {
	vehicle_t vehicles[VEHICLES_SIZE]; //TODO: musi byæ zrobiona linked list bo nie wiadomo ile aut bêdzie
	light_state_t light_state;
	uint8_t car_num;
	direction_t direction;
	//uint8_t num_of_lines; //TODO zwiêkszenie liczby pasów 
	
} road_t;

///TODO: dla wiêkszej liczby pasów
//typedef struct {
//	vehicle_t vehicles[VEHICLES_SIZE];
//	light_state_t light_state;
//	uint8_t car_num;
//}line_t;
//
//typedef struct {
//	line_t line;
//	line_t* next;
//}line_node_t;
//
//typedef struct {
//	line_t* line_head;
//	direction_t direction;

	//uint8_t num_of_lines; //TODO zwiêkszenie liczby pasów 

//} road_t;

void road_initialize(road_t* road, direction_t direction);
void add_vehicle(road_t* roads, const char* ID, direction_t startRoad, direction_t endRoad);
//void change_lights(road_t* road, direction_t direction, light_state_t light_state);
void change_lights(road_t* road, light_state_t light_state);
void cross_update(road_t* roads, char** buff);


////////////////testowe
char which_direction(direction_t direction);
char which_light(light_state_t light);
void print_left(uint8_t* left_vehicle_buff);
#endif