#include "Random.h"
#include <stdlib.h>

double Random::RandFloat()
{
	return ((rand()) / (RAND_MAX + 1.0));
}

double Random::RandInt(int iMin, int iMax)
{
	return rand() % (iMax - iMin + 1) + iMin;
}

double Random::RandClamped()
{
	return RandFloat() - RandFloat();
}