/*
* @file main.c
* @author Kapa³a Przemys³aw
* @date May 2025
* @brief
* 
*/

#include"cross.h"
#include<stdio.h>


static direction_t translate_direction(char* name);

int main() {
	char command[11];
	char vehicleID[12];
	char startRoad[6];
	char endRoad[6];
	direction_t directions[] = {N,S,E,W};
	static char* left_vehicle_buff[NUM_OF_DIRECTIONS];
	memset(left_vehicle_buff, 0, sizeof(left_vehicle_buff));
	//uint8_t lines[] = { 1,1,1,1 }; // okreœlenie ile dany kierunek ma pasów
	road_t roads[NUM_OF_DIRECTIONS];
	//inicjalizacja
	for (uint8_t i = 0; i < NUM_OF_DIRECTIONS; i++) {
		road_initialize(&roads[i], directions[i]);
	}

	while (1) {
#pragma warning(disable:4996) // wy³¹cza ostrze¿enie o strcpy
		if (scanf("%10s", command) != 1) {
			break;	//b³ad wczytania komendy
		}
#pragma warning(disable:4996) // wy³¹cza ostrze¿enie o strcpy
		if (strcmp(command, "step") == 0) {
			cross_step(roads, left_vehicle_buff);
			send_left_cars(left_vehicle_buff);
			fflush(stdout);
		}
		else if (strcmp(command, "addVehicle") == 0) {
#pragma warning(disable:4996) // wy³¹cza ostrze¿enie o strcpy
			if (scanf("%11s %5s %5s", vehicleID, startRoad, endRoad) == 3) {
				add_vehicle(roads, vehicleID, translate_direction(startRoad), translate_direction(endRoad));
				printf("Added vehicle: %s\n", vehicleID);
				fflush(stdout);
			}
		}
		else {
			printf("\n"); //nieznana komenda
			fflush(stdout);
		}
	}
	return 0;
}

static direction_t translate_direction(char* name) {
	if (strcmp(name, "north") == 0)
		return N;
	else if (strcmp(name, "south") == 0)
		return S;
	else if (strcmp(name, "west") == 0)
		return W;
	else
		return E;
}


