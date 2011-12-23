#include "game.h"

void free_creature(struct creature *creature)
{
	free(creature->name);
	creature->name = NULL;

	free(creature);
}
