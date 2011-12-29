#include "game.h"

static struct drop_table drop_table[] = {
	{90, 1, 10, NULL},
	{10, 0,  0, create_random_armor},
	{90, 0,  0, create_random_potion},
	{0,  0,  0, NULL},
};

struct creature *create_bat()
{
	int rc;
	struct creature *bat;

	bat = calloc(1, sizeof(*bat));
	assert(bat);

	bat->color = BLUE;
	bat->symbol = 'b';

	rc = asprintf(&bat->name, "a bat");
	assert(rc != -1);
	bat->health = bat->max_health = 20;
	bat->mana = bat->max_mana = 0;
	bat->strength = 3;
	bat->intelligence = 3;
	bat->dexterity = 3;
	bat->level = 1;
	bat->experience = 20;

	bat->do_hurt = creature_hurt;
	bat->attack = creature_attack;
	bat->die = creature_die;

	bat->drop_table = drop_table;

	return bat;
}

