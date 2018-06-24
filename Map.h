#ifndef __MAP_H
#define __MAP_H

#include "List.h"

#define MAX_CITY_NAME 128

typedef struct city {
    char name[MAX_CITY_NAME];
    int x;
    int y;
    List *neighbors;
    int distance_from_start;
    int distance_to_end;
} City;

typedef struct neighbor {
    struct city *city;
    char name[MAX_CITY_NAME];
    int distance;
} Neighbor;

#endif //__MAP_H
