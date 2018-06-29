#include "Astar.h"

static int compareFScore(void *val1, void *val2)
{
    City *city1 = (City *)val1;
    City *city2 = (City *)val2;

    int fScore1 = city1->distance_from_start + city1->distance_to_end;
    int fScore2 = city2->distance_from_start + city2->distance_to_end;

    return (fScore1 - fScore2);
}

static int compareName(void *val1, void *val2)
{
    return strcmp((char *)val1, (char *)val2);
}

static void printFScore(void *val)
{
    City *city = (City *)val;

    int fScore = city->distance_from_start + city->distance_to_end;

    printf("[%s] F:%d G:%d H:%d\n",
            city->name, fScore, city->distance_from_start, city->distance_to_end);
}

static int calculateHScore(City *current, City *goal)
{
    int a = current->x - goal->x;
    int b = current->y - goal->y;

    if (a == 0) a++;
    if (b == 0) b++;

    double c = sqrt((double)((a*a) + (b*b)));

    // return half of the beeline distance to the goal, just from observing the map
    return (int)(c/2);
}

int aStar(City *start, City *goal)
{
    List *open;
    List *closed;
    Node *currentCityNode;
    Node *currentNeighborNode;

    City *city;
    Neighbor *neighbor;

    int tmpDistance;

    open = newList(compareFScore, compareName, printFScore);
    closed = newList(compareFScore, compareName, printFScore);

    start->distance_from_start = 0;
    start->distance_to_end = calculateHScore(start, goal);

    addList(open, start);

    // Loop while there is still something in the open list
    while (open->head) {
        currentCityNode = open->head;   // head will always be the lowest F score
        city = (City *)currentCityNode->val;

        // Check to see if we're at the goal already
        if (strcmp(city->name, goal->name) == 0) {
            printFoundPath(goal);
            return 0;
        }

        // Move current city from open to closed
        remFromList(open, city);
        addList(closed, city);

        // Loop through each neighbor in the city
        currentNeighborNode = city->neighbors->head;
        while (currentNeighborNode) {
            neighbor = (Neighbor *)currentNeighborNode->val;

            // Skip this neighbor if it's already in the closed list
            if (isInList(closed, neighbor->city)) {
                currentNeighborNode = currentNeighborNode->next;
                continue;
            }

            // Update the "G" score aka distance from start
            tmpDistance = city->distance_from_start + neighbor->distance;

            if (tmpDistance >= neighbor->city->distance_from_start) {
                addList(closed, neighbor->city);
                currentNeighborNode = currentNeighborNode->next;
                continue;   // this neighbor will not provide a faster path
            }

            // if we're here, this neighbor is a candidate
            neighbor->city->cameFrom = city;
            neighbor->city->distance_from_start = tmpDistance;
            neighbor->city->distance_to_end = calculateHScore(neighbor->city, goal);

            // Add the neighbor city to the open list, if not already there
            if (!isInList(open, neighbor->city)) {
                addList(open, neighbor->city);
            }

            currentNeighborNode = currentNeighborNode->next;
        }
    }

    return 1;
}

static int compareDistanceFromStart(void *a, void *b)
{
    City *cityA = (City *)a;
    City *cityB = (City *)b;

    return cityA->distance_from_start - cityB->distance_from_start;
}

void printFoundPath(City *goal)
{
    City *walk = goal;
    Node *walkNode;

    List *path = newList(compareDistanceFromStart, compareName, printFScore);

    while (walk) {
        addList(path, walk);
        walk = walk->cameFrom;
    }

    walkNode = path->head;

    while (walkNode) {
        walk = (City *)walkNode->val;
        printf("%s [%d]\n", walk->name, walk->distance_from_start);
        walkNode = walkNode->next;
    }
}

static char usage[256] = "Please enter two city names: e.g. map Paris Lyon. See --help";

int main(int argc, char **argv)
{
    List *map;
    City *start;
    City *goal;
    int rc;

    map = buildMap("FRANCE.MAP");

    if (argc == 2 && (strcmp(argv[1], "--help") == 0)) {
        printf("%s\n", usage);
        printf("Possible city names are:\n");
        Node *cityNode = (Node *)map->head;
        while (cityNode) {
            printf("\t%s\n", ((City *)cityNode->val)->name);
            cityNode = cityNode->next;
        }
        return 0;
    } else if (argc < 3) {
        printf("%s\n", usage);
        return -1;
    }

    start = findCityByName(map, argv[1]);
    goal = findCityByName(map, argv[2]);

    if  (!start || !goal) {
        printf("Not acceptable city names, see --help for options\n");
        return -2;
    }

    rc = aStar(start, goal);

    return rc;
}
