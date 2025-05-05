#include"cross.h"
//#include<stdio.h>


void road_initialize(road_t* road, direction_t direction) {
		memset(road->vehicles, 0, sizeof(road->vehicles));
		road->light_state = RED_LIGHT;
		road->car_num = 0;
		road->direction = direction;
}
//je¿eli tyle samo aut na dwóch kierunkach, wybierany jest pierwszy znaleziony
direction_t busiest_direction(road_t* roads) {
	uint8_t max_car_num = 0;
	direction_t direction = N; //TODO
	for(uint8_t i = 0; i < NUM_OF_ROADS; i++) {
		if (roads[i].car_num > max_car_num) {
			max_car_num = roads[i].car_num;
			direction = roads[i].direction;
		}
	}
	return direction;
}
static turn_t which_turn(road_t* road, direction_t end_direction) {
	turn_t turn = straight;
	switch (road->direction) {
	case N:
		if (end_direction == E)
			turn = left;
		else if (end_direction == W)
			turn = right;
		break;
	case S:
		if (end_direction == W)
			turn = left;
		else if (end_direction == E)
			turn = right;
		break;
	case W:
		if (end_direction == N)
			turn = left;
		else if (end_direction == S)
			turn = right;
		break;
	case E:
		if (end_direction == S)
			turn = left;
		else if (end_direction == N)
			turn = right;
		break;
	}
	return turn;
}
void add_vehicle(road_t* roads, uint8_t ID, direction_t start_direction, direction_t end_direction) {
	
	for (uint8_t i = 0; i < NUM_OF_ROADS; i++) {
		if (roads[i].direction == start_direction) {
			roads[i].vehicles[roads[i].car_num].ID = ID;
			roads[i].vehicles[roads[i].car_num].end_direction = end_direction;
			roads[i].vehicles[roads[i].car_num].turn = which_turn(&roads[i], end_direction);
			roads[i].car_num++;
			break;	
		}
	}
}

//TODO: zwraca id, zmiana na nazwe
static uint8_t left_vehicle(road_t* road) {
	uint8_t ID = 0;
	if (road->light_state == GREEN_LIGHT) {
		if (road->car_num != 0) {
			ID = road->vehicles[0].ID;
			// tylko po to ¿e auta s¹ w buforze zwyk³ym
			for (uint8_t i = 0; i < road->car_num - 1; i++) {
				road->vehicles[i] = road->vehicles[i + 1];
			}
			//
			road->car_num--;
		}
		else
			return 0;
	}
	return ID;
}
//TODO: powinna zwracaæ step_count
void cross_update(road_t* roads, uint8_t* buff) {
	static uint8_t step_count = 0; // TODO: do wypisania pojazdów w danym kroku trzeba bêdzie zmieniæ bufor w strukture z danymi i z krokiem 
	for (uint8_t i = 0; i < NUM_OF_ROADS; i++) {
		buff[i] = left_vehicle(&roads[i]);
	}
	step_count++;
}

void change_lights(road_t* road, light_state_t light_state) {
	road->light_state = light_state;
}

void cross_step(road_t* roads, uint8_t* buff) {
	cross_update(roads, buff);
	direction_t direction = busiest_direction(roads);
	direction_t opposite_direction = roads[direction].vehicles[0].end_direction;
	if ((roads[direction].vehicles[0].turn == straight || roads[direction].vehicles[0].turn == right) && (opposite_direction == straight || opposite_direction == right)) {
		for (uint8_t i = 0; i < NUM_OF_ROADS; i++) {
			if (roads[i].direction == direction || roads[i].direction == opposite_direction)
				change_lights(&roads[i], GREEN_LIGHT);
			else
				change_lights(&roads[i], RED_LIGHT);
		}
		
		//change_lights(roads, direction, GREEN_LIGHT);
		//change_lights(roads, roads[direction].vehicles[0].end_direction, GREEN_LIGHT);
	}
	//else if()
	else {
		for (uint8_t i = 0; i < NUM_OF_ROADS; i++) {
			if(roads[i].direction == direction)
				change_lights(&roads[i], GREEN_LIGHT);
			else
				change_lights(&roads[i], RED_LIGHT);
		}
		
	}
	//roads[direction] dziala tak bez pêtli dopóki s¹ cztery drogi i wpisane s¹ w tej samej kolejnoœci jako enum i do tablicy roads
	//cross_update(roads, buff);
}



/////////////////testowe
char which_direction(direction_t direction) {
	switch (direction) {
	case N:
		return 'N';
	case S:
		return 'S';
	case W:
		return 'W';
	case E:
		return 'E';
	default:
		return 'P';
	}
}
char which_light(light_state_t light) {
	switch (light) {
	case GREEN_LIGHT:
		return 'G';
	case RED_LIGHT:
		return 'R';
	case YELLOW_LIGHT:
		return 'Y';
	}
}