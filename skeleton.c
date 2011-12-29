#include "game.h"

static struct drop_table drop_table[] = {
	{90, 1, 30, NULL},
	{10, 0,  0, create_random_armor},
	{90, 0,  0, create_random_potion},
	{0,  0,  0, NULL},
};

struct creature *create_skeleton()
{
	int rc;
	struct creature *skeleton;

	skeleton = calloc(1, sizeof(*skeleton));
	assert(skeleton);

	skeleton->color = B_WHITE;
	skeleton->symbol = 'k';

	rc = asprintf(&skeleton->name, "a skeleton");
	assert(rc != -1);
	skeleton->health = skeleton->max_health = 50;
	skeleton->mana = skeleton->max_mana = 10;
	skeleton->strength = 8;
	skeleton->intelligence = 3;
	skeleton->dexterity = 3;
	skeleton->level = 3;
	skeleton->experience = 130;

	skeleton->do_hurt = creature_hurt;
	skeleton->attack = creature_attack;
	skeleton->die = creature_die;

	skeleton->drop_table = drop_table;

	return skeleton;
}

