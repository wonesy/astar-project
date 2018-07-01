#ifndef __A_STAR
#define __A_STAR

#include "Map.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

int aStar(City *start, City *goal);

void printFoundPath(City *goal);

void displayPath(City *from, City *current);

#endif // __A_STAR
