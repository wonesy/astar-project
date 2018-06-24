#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Map.h"

static List *globalMap;

int compString(void *s1, void *s2)
{
    return strcmp((char *)s1, (char *)s2);
}

void printString(void *s)
{
    printf("%s", (char *)s);
}

static City *findCityByName(List *map, char *name)
{
    City *city = 0;
    Node *curNode = map->head;

    for (int i = 0; i < map->nelts; i++) {
        if (!curNode) {
            break;
        }

        city = (City *)(curNode->val);
        if (0 == (strcmp(city->name, name))) {
            return city;
        }
        curNode = curNode->next;
    }

    city = 0;
    return city;
}

static void linkNeighbors(void *val)
{
    Neighbor *neighbor;
    City *city = (City *)val;

    Node *curNode = city->neighbors->head;

    for (int n = 0; n < city->neighbors->nelts; n++) {
        neighbor = (Neighbor *)curNode->val;
        neighbor->city = findCityByName(globalMap, neighbor->name);
        curNode = curNode->next;
    }
}

List *buildMap(char *map_file_name)
{
    List *map;
    City *city;
    Neighbor *neighbor;

    FILE *france_map = fopen(map_file_name, "r");
    char cityName[MAX_CITY_NAME];
    int x;
    int y;
    int num;

    map = newList(compString, printString);

    while ((num = fscanf(france_map, "%s %d %d", cityName, &x, &y)) != EOF) {
        if (num == 3) {
            printf("City discovered: %s (%d, %d)\n", cityName, x, y);

            // New city is found, create a new instance and populate accordingly
            city = (City *)calloc(1, sizeof(City));
            strcpy(city->name, cityName);
            city->x = x;
            city->y = y;
            city->neighbors = newList(compString, printString);

            // Add this city to the map list
            addList(map, city);
        } else if (num == 2) {
            printf("\t%s (%d)\n", cityName, x);

            // New neighbor is found, create a new instance and populate accordingly
            neighbor = (Neighbor *)calloc(1, sizeof(Neighbor));
            strcpy(neighbor->name, cityName);
            neighbor->distance = x;

            // Add this neighbor to the neighbor list within city list
            addList(city->neighbors, neighbor);

        } else {
            printf("Error");
        }
    }

    fclose(france_map);

    forEach(map, linkNeighbors); 

    return map;
}

int main()
{
    globalMap = buildMap("FRANCE.MAP");

    return 0;
}
