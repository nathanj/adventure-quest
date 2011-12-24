#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "colors.h"

struct creature {
	char *name;
	int health, max_health;
	int mana, max_mana;
	int strength;
	int intelligence;
	int dexterity;
	int level;

	void (*attack) (struct creature *this, struct creature *other);
	void (*do_hurt) (struct creature *this, struct creature *hurter);
};

struct player {
	struct creature self;

	int gold;

	int world_level;
	int world_x;
	int world_y;

	struct item *inventory;

	void (*move) (int x, int y, int z);
};

struct item {
	char *name;
	void (*interact) (struct player *player);
};

struct armor {
	struct item item;
	int armour_class;
	int strength;
	int intelligence;
	int dexterity;
};

struct room {
	int stairs_down : 1;
	int stairs_up : 1;

	struct creature *creatures;
	struct item *items;
};

struct armor *create_random_armor();
void print_armor(struct armor *armor);

void free_creature(struct creature *creature);

struct creature *create_bat();

void init_player();

void init_world();
void print_map();
struct room *current_room();

extern struct player player;

#endif /* __GAME_H__ */

