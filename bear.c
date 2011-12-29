#include "game.h"

static struct drop_table drop_table[] = {
	{90,  70, 100, NULL},
	{70,  0,  0,   create_random_armor},
	{100, 0,  0,   create_random_weapon},
	{0,   0,  0,   NULL},
};


struct creature *create_bear()
{
	int rc;
	struct creature *bear;

	bear = calloc(1, sizeof(*bear));
	assert(bear);

	bear->color = YELLOW;
	bear->symbol = 'B';

	rc = asprintf(&bear->name, "a bear");
	assert(rc != -1);
	bear->health = bear->max_health = 100;
	bear->mana = bear->max_mana = 10;
	bear->strength = 20;
	bear->intelligence = 3;
	bear->dexterity = 3;
	bear->level = 5;
	bear->experience = 200;

	bear->do_hurt = creature_hurt;
	bear->attack = creature_attack;
	bear->die = creature_die;

	bear->drop_table = drop_table;

	return bear;
}

