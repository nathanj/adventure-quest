#include "game.h"

/**
 * p - returns true or false with a given probability
 */
int p(int percent)
{
	return ((rand() % 100) + 1 <= percent);
}
