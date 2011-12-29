#include "game.h"

/**
 * p - returns true or false with a given probability
 */
int p(int percent)
{
	return ((rand() % 100) + 1 <= percent);
}

/**
 * rand_between - returns a random number in [a,b)
 */
int rand_between(int a, int b)
{
	return rand() % (b - a) + a;
}
