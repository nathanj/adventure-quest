#include "game.h"

static struct drop_table drop_table[] = {
	{90, 10, 20, NULL},
	{20, 0,  0,  create_random_armor},
	{70, 0,  0,  create_random_potion},
	{0,  0,  0, NULL},
};

struct creature *create_slime()
{
	int rc;
	struct creature *slime;

	slime = calloc(1, sizeof(*slime));
	assert(slime);

	slime->color = GREEN;
	slime->symbol = 's';

	rc = asprintf(&slime->name, "a slime");
	assert(rc != -1);
	slime->health = slime->max_health = 50;
	slime->mana = slime->max_mana = 0;
	slime->strength = 4;
	slime->intelligence = 3;
	slime->dexterity = 3;
	slime->level = 2;
	slime->experience = 70;

	slime->do_hurt = creature_hurt;
	slime->attack = creature_attack;
	slime->die = creature_die;

	slime->drop_table = drop_table;

	return slime;
}

