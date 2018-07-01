#ifndef __MAP_H
#define __MAP_H

#include "List.h"

#define MAX_CITY_NAME 128
#define MAX_DISTANCE 10000

typedef struct city {
    char name[MAX_CITY_NAME];
    int x;
    int y;
    List *neighbors;            // succ
    int distance_from_start;    // g
    int distance_to_end;        // h
    struct city *cameFrom;
} City;

typedef struct neighbor {
    struct city *city;
    char name[MAX_CITY_NAME];
    int distance;
} Neighbor;

List *buildMap(char *map_file_name);

void prettyPrintCity(void *val);

City *findCityByName(List *map, char *name);

#endif //__MAP_H
