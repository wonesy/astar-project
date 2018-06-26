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

void aStar(City *start, City *goal)
{
    List *open;
    List *closed;
    Node *currentCityNode;
    Node *currentNeighborNode;

    City *city;
    Neighbor *neighbor;

    open = newList(compareFScore, compareName, printFScore);
    closed = newList(compareFScore, compareName, printFScore);

    start->distance_from_start = 0;
    start->distance_to_end = calculateHScore(start, goal);
    
    addList(open, start);

    // Loop while there is still something in the open list
    while (open->head) {
        currentCityNode = open->head;   // head will always be the lowest F score
        city = (City *)currentCityNode->val;
        printFScore(city);

        // Check to see if we're at the goal already
        if (city->name == goal->name) {
            // we've reached the goal and can end
            // TODO XXX
            return;
        }

        // Move current city from open to closed
        remFromListAt(open, 0, (void **)&city);
        addList(closed, city);

        currentNeighborNode = city->neighbors->head;
        while (currentNeighborNode) {
            neighbor = (Neighbor *)currentNeighborNode->val;

            // Skip this neighbor if it's already in the closed list
            if (isInList(closed, neighbor->city)) {
                continue;
            }

            // Add the neighbor city to the open list, if not already there
            if (!isInList(open, neighbor->city)) {
                addList(open, neighbor->city);
            }

            currentNeighborNode = currentNeighborNode->next; 
        }

    }
}

