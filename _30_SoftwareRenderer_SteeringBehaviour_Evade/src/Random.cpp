#include "Random.h"
#include <stdlib.h>

// Returns a random double int the range 0.0 < n < 1.0
double Random::RandFloat()
{
	return ((rand()) / (RAND_MAX + 1.0));
}

// Returns a random integer int the range iMin < n < iMax
double Random::RandInt(int iMin, int iMax)
{
	return rand() % (iMax - iMin + 1) + iMin;
}

// Returns a random double in the range -1.0 < n < 1.0
double Random::RandClamped()
{
	return RandFloat() - RandFloat();
}