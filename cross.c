/*
* @file cross.c
* @author Kapa³a Przemys³aw
* @date May 2025
* @brief
*
*/

#include "cross.h"
#include <string.h>
#include <stdlib.h>
#include<stdio.h>


void road_initialize(road_t* road, direction_t direction) {
	//memset(road->vehicles, 0, sizeof(road->vehicles));
	road->main_light_state = RED_LIGHT;
	road->right_light_state = RED_LIGHT;
	road->car_num = 0;
	road->waiting_steps = 0;
	road->direction = direction;
	road->head = NULL;
}
//je¿eli tyle samo aut na dwóch kierunkach, wybierany jest pierwszy znaleziony
static direction_t busiest_direction(road_t* roads) {
	uint8_t max_car_num = 0;
	direction_t direction = N; //TODO
	for (uint8_t i = 0; i < NUM_OF_DIRECTIONS; i++) {
		if (roads[i].car_num > max_car_num) {
			max_car_num = roads[i].car_num;
			direction = roads[i].direction;
		}
	}
	return direction;
}
static direction_t which_is_left_direction(road_t* road) {
	switch (road->direction) {
	case N:
		return E;
	case S:
		return W;
	case W:
		return N;
	case E:
		return S;
	}
}

static direction_t which_is_opposite_direction(road_t* road) {
	switch (road->direction) {
	case N:
		return S;
	case S:
		return N;
	case W:
		return E;
	case E:
		return W;
	}
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
static void add_vehicle_to_list(vehicle_node_t** head, const char* ID, turn_t turn, direction_t end_direction) {
	if (*head == NULL) {
		*head = (vehicle_node_t*)malloc(sizeof(vehicle_node_t));
		(*head)->vehicle.end_direction = end_direction;
		(*head)->vehicle.ID = malloc(strlen(ID) + 1);
#pragma warning(disable:4996) // wy³¹cza ostrze¿enie o strcpy
		strcpy((*head)->vehicle.ID, ID);
		(*head)->vehicle.turn = turn;
		(*head)->next = NULL;
	}
	else {
		vehicle_node_t* current = *head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = (vehicle_node_t*)malloc(sizeof(vehicle_node_t));
		current->next->vehicle.end_direction = end_direction;
		current->next->vehicle.ID = malloc(strlen(ID) + 1);
		strcpy(current->next->vehicle.ID, ID);
		current->next->vehicle.turn = turn;
		current->next->next = NULL;
	}
}
static void remove_vehicle_from_list(vehicle_node_t** head, char** ID) {
	vehicle_node_t* tmp = NULL;
	if ((*head) != NULL) {
		*ID = (*head)->vehicle.ID;
		tmp = (*head)->next;
		free(*head);
		*head = tmp;
	}

}

void add_vehicle(road_t* roads, const char* ID, direction_t start_direction, direction_t end_direction) {

	for (uint8_t i = 0; i < NUM_OF_DIRECTIONS; i++) {
		if (roads[i].direction == start_direction) {
			add_vehicle_to_list(&roads[i].head, ID, which_turn(&roads[i], end_direction), end_direction);
			roads[i].car_num++;
			break;
		}
	}
}

static char* left_vehicle(road_t* road) {
	char* ID = NULL;
	if (road->car_num > 0) {
		if (road->main_light_state == GREEN_LIGHT) {
			if (road->car_num > 0) {
				remove_vehicle_from_list(&(road->head), &ID);
				road->car_num--;
				road->waiting_steps = 0;
			}
		}
		else if (road->right_light_state == GREEN_LIGHT) {
			if (road->head->vehicle.turn == right) {
				remove_vehicle_from_list(&(road->head), &ID);
				road->car_num--;
				road->waiting_steps = 0;
			}
		}
	}
	else
		road->waiting_steps = 0; //nie ma kto odjechaæ, wiêc nikt nie czeka
	return ID;
}


//void add_vehicle(road_t* roads, const char* ID, direction_t start_direction, direction_t end_direction) {
//	
//	for (uint8_t i = 0; i < NUM_OF_DIRECTIONS; i++) {
//		if (roads[i].direction == start_direction) {
//			roads[i].vehicles[roads[i].car_num].ID = malloc(strlen(ID)+1);
//			if(roads[i].vehicles[roads[i].car_num].ID != NULL)
//#pragma warning(disable:4996) // wy³¹cza ostrze¿enie o strcpy
//				strcpy(roads[i].vehicles[roads[i].car_num].ID, ID);
//			roads[i].vehicles[roads[i].car_num].end_direction = end_direction;
//			roads[i].vehicles[roads[i].car_num].turn = which_turn(&roads[i], end_direction);
//			roads[i].car_num++;
//			break;	
//		}
//	}
//}

//TODO: zwraca wskaŸnik na dynamicznie zaalokowan¹ pamiêæ z ID pojazdu, nale¿y pamiêtaæ o jej zwolnieniu
//static char* left_vehicle(road_t* road) {
//	char* ID = NULL;
//	if (road->car_num > 0) {
//		if (road->main_light_state == GREEN_LIGHT) {
//			if (road->car_num > 0) {
//				ID = road->vehicles[0].ID;
//				// tylko po to ¿e auta s¹ w buforze zwyk³ym
//				for (uint8_t i = 0; i < road->car_num - 1; i++) {
//					road->vehicles[i] = road->vehicles[i + 1];
//				}
//				//
//				road->car_num--;
//				road->waiting_steps = 0;
//			}
//		}
//		else if (road->right_light_state == GREEN_LIGHT) {
//			if (road->vehicles[0].turn == right) {
//				ID = road->vehicles[0].ID;
//				// tylko po to ¿e auta s¹ w buforze zwyk³ym
//				for (uint8_t i = 0; i < road->car_num - 1; i++) {
//					road->vehicles[i] = road->vehicles[i + 1];
//				}
//				//
//				road->car_num--;
//				road->waiting_steps = 0;
//			}
//		}
//	}
//	else
//		road->waiting_steps = 0; //nie ma kto odjechaæ, wiêc nikt nie czeka
//	return ID;
//}
//TODO: powinna zwracaæ step_count
static void cross_update(road_t* roads, char** buff) {
	//static uint8_t step_count = 0; // TODO: do wypisania pojazdów w danym kroku trzeba bêdzie zmieniæ bufor w strukture z danymi i z krokiem 
	for (uint8_t i = 0; i < NUM_OF_DIRECTIONS; i++) {
		roads[i].waiting_steps++;
		buff[i] = left_vehicle(&roads[i]);
	}
	//step_count++;
}

static void change_lights(road_t* road, light_state_t light_state) {
	road->main_light_state = light_state;
}
static void change_right_lights(road_t* road, light_state_t light_state) {
	road->right_light_state = light_state;
}
static void transient_light_state(road_t* roads) {
	for (uint8_t i = 0; i < NUM_OF_DIRECTIONS; i++) {
		if (roads[i].main_light_state == GREEN_LIGHT)
			change_lights(&roads[i], YELLOW_LIGHT);
	}
}
static void red_light_state(road_t* roads) {
	for (uint8_t i = 0; i < NUM_OF_DIRECTIONS; i++) {
		change_lights(&roads[i], RED_LIGHT);
		change_right_lights(&roads[i], RED_LIGHT);
	}
}
static void main_green_light_state(road_t* roads) {
	for (uint8_t i = 0; i < NUM_OF_DIRECTIONS; i++) {
		if (roads[i].main_light_state == YELLOW_LIGHT) {
			change_lights(&roads[i], GREEN_LIGHT);
		}
	}
}

void cross_step(road_t* roads, char** buff) {
	//cross_update(roads, buff);
	direction_t direction = N;	//default value
	bool long_waiting = false;
	uint8_t waiting = WAITING_STEPS;
	for (uint8_t i = 0; i < NUM_OF_DIRECTIONS; i++) {
		if (roads[i].waiting_steps > waiting) {
			waiting = roads[i].waiting_steps;
			direction = roads[i].direction;
			long_waiting = true;
		}
	}
	if (!long_waiting)	//je¿eli nie ma kierunku które wystarczaj¹co d³ugo oczekuje, wybierany jest kierunek z najwiêksz¹ liczb¹ aut
		direction = busiest_direction(roads);


	//Gdy na skrzy¿owaniu nie ma ju¿ ¿adnych aut a symulacja jest dalej wykonywana, zielone œwiat³o tylko na kierunku N (domyœlnym)
	if (roads[direction].car_num == 0) {
		//zmiana œwiate³ na ¿ó³te, tam gdzie wystêpowa³ ruch
		transient_light_state(roads);
		//zmiana ¿ó³tych œwiate³ na czerwone oraz wy³¹czenie œwiate³ warunkowych
		red_light_state(roads);
		change_lights(&roads[direction], YELLOW_LIGHT); //g³ówny kierunek ma zielone œwiat³o
		main_green_light_state(roads);
	}
	else {
		direction_t target_direction = roads[direction].head->vehicle.end_direction;
		direction_t left_direction = which_is_left_direction(&roads[direction]);

		switch (roads[direction].head->vehicle.turn) {
		case left:
			//zmiana œwiate³ na ¿ó³te, tam gdzie wystêpowa³ ruch
			transient_light_state(roads);
			//zmiana ¿ó³tych œwiate³ na czerwone oraz wy³¹czenie œwiate³ warunkowych
			red_light_state(roads);
			change_lights(&roads[direction], YELLOW_LIGHT); //g³ówny kierunek ma zielone œwiat³o
			change_right_lights(&roads[target_direction], GREEN_LIGHT); //kierunek do którego jedzie mo¿e warunkowo jechaæ w prawo
			main_green_light_state(roads);
			break;

		case straight:
			//zmiana œwiate³ na ¿ó³te, tam gdzie wystêpowa³ ruch
			transient_light_state(roads);
			//zmiana ¿ó³tych œwiate³ na czerwone oraz wy³¹czenie œwiate³ warunkowych
			red_light_state(roads);
			change_lights(&roads[direction], YELLOW_LIGHT);

			//przeciwleg³a droga ma zwyk³e zielone œwiat³o je¿eli tylko nie ma auta robi¹cego lewoskrêt, droga podrzêdna
			//do kierunku na wprost mo¿e mieæ ewentualnie zielone œwiat³o warunkowe w prawo
			if (roads[target_direction].car_num > 0) {
				if (roads[target_direction].head->vehicle.turn == straight || roads[target_direction].head->vehicle.turn == right)
					change_lights(&roads[target_direction], YELLOW_LIGHT);
				if (roads[target_direction].head->vehicle.turn == right)
					change_right_lights(&roads[left_direction], GREEN_LIGHT);
			}
			else
				change_right_lights(&roads[left_direction], GREEN_LIGHT);
			main_green_light_state(roads);
			break;

		case right:
			direction_t opposite_direction = which_is_opposite_direction(&roads[direction]);
			//zmiana œwiate³ na ¿ó³te, tam gdzie wystêpowa³ ruch
			transient_light_state(roads);
			//zmiana ¿ó³tych œwiate³ na czerwone oraz wy³¹czenie œwiate³ warunkowych
			red_light_state(roads);
			change_lights(&roads[direction], YELLOW_LIGHT);
			change_right_lights(&roads[target_direction], GREEN_LIGHT);

			if (roads[opposite_direction].car_num > 0) {
				if (roads[opposite_direction].head->vehicle.turn == straight || roads[opposite_direction].head->vehicle.turn == right)
					change_lights(&roads[opposite_direction], YELLOW_LIGHT);
				if (roads[opposite_direction].head->vehicle.turn == right)
					change_right_lights(&roads[left_direction], GREEN_LIGHT);
			}
			else
				change_right_lights(&roads[left_direction], GREEN_LIGHT);
			main_green_light_state(roads);
			break;
		}
	}
	cross_update(roads, buff);
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
	default:
		return 'O';
	}
}
//funkcja do przekazania wy¿ej jakie pojazdy odjecha³y// zwalnianie pamiêci !!!
void print_left(char** left_vehicle_buff) {
	printf("-----------------------------------------------------\n");
	for (uint8_t i = 0; i < NUM_OF_DIRECTIONS; i++) {
		if (left_vehicle_buff[i] != 0) {
			printf("Opuscil: %s z kierunku: %c\n", left_vehicle_buff[i], which_direction(i));
			free(left_vehicle_buff[i]);
		}
	}
	memset(left_vehicle_buff, 0, sizeof(left_vehicle_buff));
	printf("-----------------------------------------------------\n");
}
void send_left_cars(char** left_vehicle_buff) {
	for (uint8_t i = 0; i < NUM_OF_DIRECTIONS; i++) {
		if (left_vehicle_buff[i] != 0) {
			printf("%s ", left_vehicle_buff[i]);
			free(left_vehicle_buff[i]);
		}
	}
	printf("\n");
	memset(left_vehicle_buff, 0, sizeof(left_vehicle_buff));
}